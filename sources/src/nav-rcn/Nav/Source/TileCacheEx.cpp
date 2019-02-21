#include "DetourEx.h"
#include "DetourTileCache.h"
#include <sstream>

extern "C"
{
	// ≥ı ºªØ
	EXPORT_API dtTileCache* ex_dtAllocTileCache(dtTileCacheParams* params)
	{
		dtTileCache* ret = dtAllocTileCache();
		if (!ret)
			return ret;
		//_allocator = new (std::nothrow) LinearAllocator(32000);
		//_compressor = new (std::nothrow) FastLZCompressor;
		//_meshProcess = new (std::nothrow) MeshProcess(_geomData);
		//ret->init(params, _allocator, _compressor, _meshProcess);
		return ret;
	}

	//  Õ∑≈
	EXPORT_API void ex_dtFreeTileCache(dtTileCache* tc)
	{
		dtFreeTileCache(tc);
	}
}