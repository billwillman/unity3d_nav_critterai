using System;
using System.Runtime.InteropServices;
using org.critterai.nav.rcn;
#if NUNITY
using Vector3 = org.critterai.Vector3;
#else
using Vector3 = UnityEngine.Vector3;
#endif

[StructLayout(LayoutKind.Sequential)]
public struct dtTileCacheParams {
    public Vector3 orig;
    public float cs, ch;
    public int width, height;
    public float walkableHeight;
    public float walkableRadius;
    public float walkableClimb;
    public float maxSimplificationError;
    public int maxTiles;
    public int maxObstacles;
};

namespace org.critterai.nav {

    public sealed class TileCache {
        private IntPtr m_Ptr = IntPtr.Zero;
        private IntPtr m_AllocPtr = IntPtr.Zero;

        /// <summary>
        /// 初始化TileCache
        /// </summary>
        /// <param name="navMesh">地形navMesh</param>
        /// <param name="pms">TileCache参数</param>
        public TileCache(IntPtr navMesh, dtTileCacheParams pms) {
            m_AllocPtr = TileCacheEx.ex_dtCreateTileCacheAllocator();
            m_Ptr = TileCacheEx.ex_dtAllocTileCache(navMesh, ref pms, m_AllocPtr);
        }

        /// <summary>
        /// 释放，清理C++对象内存
        /// </summary>
        public void Dispose() {
            if (m_Ptr != IntPtr.Zero) {
                TileCacheEx.ex_dtFreeTileCache(m_Ptr);
                m_Ptr = IntPtr.Zero;
            }

            if (m_AllocPtr != IntPtr.Zero) {
                TileCacheEx.ex_dtFreeTileCacheAllocator(m_AllocPtr);
                m_AllocPtr = IntPtr.Zero;
            }
        }

        /// <summary>
        /// 添加动态圆柱体阻挡物体
        /// </summary>
        /// <param name="pos">世界坐标系位置</param>
        /// <param name="radius">圆柱体半径</param>
        /// <param name="height">高度</param>
        /// <param name="id">返回阻挡物ID标识</param>
        /// <returns>是否添加成功</returns>
        public bool AddObstacle(Vector3 pos, float radius, float height, out uint id) {
            id = 0;
            if (m_Ptr == IntPtr.Zero)
                return false;
            return TileCacheEx.ex_dtAddObstacle(m_Ptr, ref pos, radius, height, ref id);
        }

        /// <summary>
        /// 添加BOX类动态碰撞体
        /// </summary>
        /// <param name="bmin">世界坐标系最小位置</param>
        /// <param name="bmax">世界坐标系最大位置</param>
        /// <param name="id">返回动态碰撞ID标识</param>
        /// <returns>是否添加成功</returns>
        public bool AddBoxObstacle(Vector3 bmin, Vector3 bmax, out uint id) {
            id = 0;
            if (m_Ptr == IntPtr.Zero)
                return false;
            return TileCacheEx.ex_dtAddBoxObstacle(m_Ptr, ref bmin, ref bmax, ref id);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="center"></param>
        /// <param name="halfExtents"></param>
        /// <param name="yRadians"></param>
        /// <param name="id"></param>
        /// <returns></returns>
        public bool AddBoxObstacle(Vector3 center, Vector3 halfExtents, float yRadians, out uint id) {
            id = 0;
            if (m_Ptr == IntPtr.Zero)
                return false;
            return TileCacheEx.ex_dtAddBoxObstacle2(m_Ptr, ref center, ref halfExtents, yRadians, ref id);
        }

        /// <summary>
        /// 删除动态阻挡物体
        /// </summary>
        /// <param name="id">标识</param>
        /// <returns>是否删除成功</returns>
        public bool RemoveObstacle(uint id) {
            if (m_Ptr == IntPtr.Zero)
                return false;
            return TileCacheEx.ex_dtRemoveObstacle(m_Ptr, id);
        }
    }
}
