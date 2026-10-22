/*
 * A337033  The number of n-step self avoiding walks on a 3D cubic lattice
 *          confined inside a box of size 2x2x2 where the walk starts at the
 *          center of one of the box's faces.
 * https://oeis.org/A337033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   a(n) = number of n‑step self‑avoiding walks starting at the
 *            centre of a face * of a 2×2×2 cubic lattice. The values are
 *            obtained by exhaustive DFS enumeration * of all walks up to
 *            length 26; for n≥27 the result is 0 (no walk can * visit more
 *            than 27 distinct lattice points)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   coord_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/coord_to_idx.h"

static const int dirs[6][3] = {
    { 1, 0, 0 }, { -1, 0, 0 },
    { 0, 1, 0 }, { 0, -1, 0 },
    { 0, 0, 1 }, { 0, 0, -1 }
};

/* Pre‑computed neighbours for each of the 27 lattice points. */
static unsigned char neigh_cnt[27];
static int neigh_idx[27][6];
static seq_t walk_cnt[27];
static bool precomputed = false;

/* Recursive DFS that counts walks of all lengths up to 26. */
static void dfs_walk(int cur, int depth, uint32_t visited)
{
    walk_cnt[depth]++;

    if (depth == 26)
        return;

    /* For each neighbour that has not yet been visited, continue the walk. */
    for (unsigned char k = 0; k < neigh_cnt[cur]; ++k) {
        int nxt = neigh_idx[cur][k];
        uint32_t bit = 1u << nxt;
        if (visited & bit)
            continue;
        dfs_walk(nxt, depth + 1, visited | bit);
    }
}

/* One‑time initialisation of neighbour lists and walk counts. */
static void initialise(void)
{
    if (precomputed)
        return;

    /* Build neighbour lists. */
    for (int z = 0; z < 3; ++z) {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                int idx = coord_to_idx(x, y, z);
                unsigned char cnt = 0;
                for (int d = 0; d < 6; ++d) {
                    int nx = x + dirs[d][0];
                    int ny = y + dirs[d][1];
                    int nz = z + dirs[d][2];
                    if (nx < 0 || nx > 2 || ny < 0 || ny > 2 || nz < 0 || nz > 2)
                        continue;
                    neigh_idx[idx][cnt++] = coord_to_idx(nx, ny, nz);
                }
                neigh_cnt[idx] = cnt;
            }
        }
    }

    /* The walk starts at the centre of a face, e.g. (1,1,0). */
    int start_idx = coord_to_idx(1, 1, 0);
    dfs_walk(start_idx, 0, 1u << start_idx);

    precomputed = true;
}

seq_t a(idx_t n)
{
    initialise();
    if (n >= 27)
        return (seq_t)0;
    return walk_cnt[(size_t)n];
}
