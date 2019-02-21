#include "TileCacheUtil.h"
#include "DetourTileCache.h"
#include <sstream>
#include "fastlz.h"

LinearAllocator::LinearAllocator(const int cap)
	: buffer(nullptr)
	, capacity(0)
	, top(0)
	, high(0)
{
	resize(cap);
}

LinearAllocator::~LinearAllocator()
{
	dtFree(buffer);
}

void LinearAllocator::free(void* /*ptr*/)
{

}

void* LinearAllocator::alloc(const int size)
{
	if (!buffer)
		return nullptr;
	if (top + size > capacity)
		return nullptr;
	unsigned char* mem = &buffer[top];
	top += size;
	return mem;
}

void LinearAllocator::reset()
{
	high = dtMax(high, top);
	top = 0;
}

void LinearAllocator::resize(const int cap)
{
	if (buffer) dtFree(buffer);
	buffer = (unsigned char*)dtAlloc(cap, DT_ALLOC_PERM);
	capacity = cap;
}

int FastLZCompressor::maxCompressedSize(const int bufferSize)
{
	return (int)(bufferSize* 1.05f);
}

dtStatus FastLZCompressor::decompress(const unsigned char* compressed, const int compressedSize
	, unsigned char* buffer, const int maxBufferSize, int* bufferSize)
{
	*bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
	return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
}

dtStatus FastLZCompressor::compress(const unsigned char* buffer, const int bufferSize
	, unsigned char* compressed, const int /*maxCompressedSize*/, int* compressedSize)
{
	*compressedSize = fastlz_compress((const void *const)buffer, bufferSize, compressed);
	return DT_SUCCESS;
}

MeshProcess::MeshProcess(const GeomData *geom)
	: data(geom)
{
}

MeshProcess::~MeshProcess()
{

}

void MeshProcess::process(pdtNavMeshCreateParams params
	, unsigned char* polyAreas, unsigned short* polyFlags)
{
	// Update poly flags from areas.
	for (int i = 0; i < params->polyCount; ++i)
	{
		if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
			polyAreas[i] = 0;

		if (polyAreas[i] == 0)
			polyFlags[i] = 1;

		//if (polyAreas[i] == SAMPLE_POLYAREA_GROUND ||
		//	polyAreas[i] == SAMPLE_POLYAREA_GRASS ||
		//	polyAreas[i] == SAMPLE_POLYAREA_ROAD)
		//{
		//	polyFlags[i] = SAMPLE_POLYFLAGS_WALK;
		//}
		//else if (polyAreas[i] == SAMPLE_POLYAREA_WATER)
		//{
		//	polyFlags[i] = SAMPLE_POLYFLAGS_SWIM;
		//}
		//else if (polyAreas[i] == SAMPLE_POLYAREA_DOOR)
		//{
		//	polyFlags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
		//}
	}

	// Pass in off-mesh connections.
	params->offMeshConVerts = data->offMeshConVerts;
	params->offMeshConRad = data->offMeshConRads;
	params->offMeshConDir = data->offMeshConDirs;
	params->offMeshConAreas = data->offMeshConAreas;
	params->offMeshConFlags = data->offMeshConFlags;
	params->offMeshConUserID = data->offMeshConId;
	params->offMeshConCount = data->offMeshConCount;
}

TileCacheAllocator::TileCacheAllocator()
{
	_allocator = new (std::nothrow) LinearAllocator(32000);
	_compressor = new (std::nothrow) FastLZCompressor;

	_geomData = new (std::nothrow) GeomData;
	_geomData->offMeshConCount = 0;

	_meshProcess = new (std::nothrow) MeshProcess(_geomData);
}

TileCacheAllocator::~TileCacheAllocator()
{
	if (_geomData)
	{
		delete _geomData;
		_geomData = NULL;
	}
}