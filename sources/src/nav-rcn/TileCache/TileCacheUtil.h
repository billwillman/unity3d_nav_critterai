#pragma once
#include "DetourCommon.h"
#include "DetourTileCacheBuilder.h"
#include "DetourTileCache.h"

struct LinearAllocator : public dtTileCacheAlloc
{
	unsigned char* buffer;
	int capacity;
	int top;
	int high;

	LinearAllocator(const int cap);

	~LinearAllocator();

	void resize(const int cap);

	virtual void reset();

	virtual void* alloc(const int size);

	virtual void free(void* /*ptr*/);
};

struct FastLZCompressor : public dtTileCacheCompressor
{
	virtual int maxCompressedSize(const int bufferSize);

	virtual dtStatus compress(const unsigned char* buffer, const int bufferSize,
		unsigned char* compressed, const int /*maxCompressedSize*/, int* compressedSize);

	virtual dtStatus decompress(const unsigned char* compressed, const int compressedSize,
		unsigned char* buffer, const int maxBufferSize, int* bufferSize);
};

struct GeomData
{
	static const int MAX_OFFMESH_CONNECTIONS = 256;
	float offMeshConVerts[MAX_OFFMESH_CONNECTIONS * 3 * 2];
	float offMeshConRads[MAX_OFFMESH_CONNECTIONS];
	unsigned char offMeshConDirs[MAX_OFFMESH_CONNECTIONS];
	unsigned char offMeshConAreas[MAX_OFFMESH_CONNECTIONS];
	unsigned short offMeshConFlags[MAX_OFFMESH_CONNECTIONS];
	unsigned int offMeshConId[MAX_OFFMESH_CONNECTIONS];
	int offMeshConCount;
};

struct MeshProcess : public dtTileCacheMeshProcess
{
	const GeomData *data;

	MeshProcess(const GeomData *geom);
	virtual ~MeshProcess();

	//void init(InputGeom* geom)
	//{
	//	m_geom = geom;
	//}

	virtual void process(pdtNavMeshCreateParams params,
		unsigned char* polyAreas, unsigned short* polyFlags) override;
};

// Êý¾Ý
struct TileCacheAllocator
{
	LinearAllocator *_allocator;
	FastLZCompressor *_compressor;
	MeshProcess *_meshProcess;
	GeomData *_geomData;

	TileCacheAllocator();
	~TileCacheAllocator();
};