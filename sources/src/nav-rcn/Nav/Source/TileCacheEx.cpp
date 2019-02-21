#include "DetourEx.h"
#include "DetourTileCache.h"
#include "TileCacheUtil.h"
#include <sstream>

extern "C"
{
	// ³õÊ¼»¯
	EXPORT_API dtTileCache* ex_dtAllocTileCache(dtNavMesh* navMesh, dtTileCacheParams* params, TileCacheAllocator* alloc)
	{
		if (!navMesh || !params || !alloc)
			return NULL;
		dtTileCache* ret = dtAllocTileCache();
		if (!ret)
			return ret;
		dtStatus status = ret->init(params, alloc->_allocator, alloc->_compressor, alloc->_meshProcess);

		if (dtStatusFailed(status))
		{
			if (ret)
			{
				dtFreeTileCache(ret);
				ret = NULL;
			}
			return ret;
		}

		/*
		dtCompressedTileRef tileRef = DT_COMPRESSEDTILE_FREE_DATA;
		status = ret->buildNavMeshTile(tileRef, navMesh);

		if (dtStatusFailed(status))
		{
			if (ret)
			{
				dtFreeTileCache(ret);
				ret = NULL;
			}
			return ret;
		}
		*/

		return ret;
	}

	// ÊÍ·Å
	EXPORT_API void ex_dtFreeTileCache(dtTileCache* tc)
	{
		dtFreeTileCache(tc);
	}

	EXPORT_API TileCacheAllocator* ex_dtCreateTileCacheAllocator()
	{
		TileCacheAllocator* ret = new TileCacheAllocator();
		return ret;
	}

	EXPORT_API void ex_dtFreeTileCacheAllocator(TileCacheAllocator* alloc)
	{
		if (alloc)
		{
			delete alloc;
			alloc = NULL;
		}
	}

	EXPORT_API dtStatus ex_dtAddObstacle(dtTileCache* owner, const float* pos, const float radius, const float height, dtObstacleRef* result)
	{
		return owner->addObstacle(pos, radius, height, result);
	}

	EXPORT_API dtStatus ex_dtAddBoxObstacle(dtTileCache* owner, const float* bmin, const float* bmax, dtObstacleRef* result)
	{
		return owner->addBoxObstacle(bmin, bmax, result);
	}

	EXPORT_API dtStatus ex_dtAddBoxObstacle2(dtTileCache* owner, const float* center, const float* halfExtents, const float yRadians, dtObstacleRef* result)
	{
		return owner->addBoxObstacle(center, halfExtents, yRadians, result);
	}

	EXPORT_API dtStatus ex_dtRemoveObstacle(dtTileCache* owner, const dtObstacleRef ref)
	{
		return owner->removeObstacle(ref);
	}

	EXPORT_API dtStatus ex_dtTileCacheUpdate(dtNavMesh* navMesh, dtTileCache* owner, float dt)
	{
		return owner->update(dt, navMesh);
	}
}

