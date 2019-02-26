#include "DetourEx.h"
#include "DetourTileCache.h"
#include "TileCacheUtil.h"
#include <sstream>

extern "C"
{
	// 初始化
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

	// 释放
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

	EXPORT_API bool ex_dtAddObstacle(dtTileCache* owner, const float* pos, const float radius, const float height, dtObstacleRef* result)
	{
		dtStatus status = owner->addObstacle(pos, radius, height, result);
		return dtStatusSucceed(status);
	}

	EXPORT_API bool ex_dtAddBoxObstacle(dtTileCache* owner, const float* bmin, const float* bmax, dtObstacleRef* result)
	{
		dtStatus status = owner->addBoxObstacle(bmin, bmax, result);
		return dtStatusSucceed(status);
	}

	EXPORT_API bool ex_dtAddBoxObstacle2(dtTileCache* owner, const float* center, const float* halfExtents, const float yRadians, dtObstacleRef* result)
	{
		dtStatus status = owner->addBoxObstacle(center, halfExtents, yRadians, result);
		return dtStatusSucceed(status);
	}

	EXPORT_API bool ex_dtRemoveObstacle(dtTileCache* owner, const dtObstacleRef ref)
	{
		dtStatus status = owner->removeObstacle(ref);
		return dtStatusSucceed(status);
	}

	EXPORT_API bool ex_dtTileCacheUpdate(dtNavMesh* navMesh, dtTileCache* owner, float dt)
	{
		dtStatus status = owner->update(dt, navMesh);
		return dtStatusSucceed(status);
	}

	// 从NAVMESH到PARAMS
	EXPORT_API bool ex_dtNavMeshToTileCacheParam(dtNavMesh* navMesh, dtTileCacheParams* params)
	{
		if (!navMesh || !params)
			return false;
		const float* orgSrc = navMesh->getOrig();
		if (!orgSrc)
			return false;
		memcpy(params->orig, orgSrc, sizeof(float) * 3);

		return true;
	}

}

