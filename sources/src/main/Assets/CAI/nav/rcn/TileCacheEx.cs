using System;
using System.Runtime.InteropServices;
#if NUNITY
using Vector3 = org.critterai.Vector3;
#else
using Vector3 = UnityEngine.Vector3;
#endif

namespace org.critterai.nav.rcn {
    // 处理动态碰撞
    internal static class TileCacheEx {
        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern IntPtr ex_dtAllocTileCache(IntPtr navMesh, ref dtTileCacheParams tileCacheParams, IntPtr alloc);

        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern void ex_dtFreeTileCache(IntPtr tileCache);

        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern IntPtr ex_dtCreateTileCacheAllocator();

        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern void ex_dtFreeTileCacheAllocator(IntPtr tileCacheAlloc);

        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern bool ex_dtAddObstacle(IntPtr tileCache, ref Vector3 pos, float radius, float height, ref uint result);

        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern bool ex_dtAddBoxObstacle(IntPtr tileCache, ref Vector3 bmin, ref Vector3 bmax, ref uint result);

        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern bool ex_dtAddBoxObstacle2(IntPtr tileCache, ref Vector3 center, ref Vector3 halfExtents, float yRadians, ref uint result);

        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern bool ex_dtRemoveObstacle(IntPtr tileCache, uint obstacleRef);

        [DllImport(InteropUtil.PLATFORM_DLL)]
        public static extern bool ex_dtTileCacheUpdate(IntPtr navMesh, IntPtr tileCache, float dt);
    }
}
