/*
 * A337034  The number of n-step self avoiding walks on a 3D cubic lattice
 *          confined inside a box of size 2x2x2 where the walk starts at one
 *          of the box's corners.
 * https://oeis.org/A337034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   This implementation counts self‑avoiding walks of length n *
 *            inside a 3×3×3 lattice (27 sites) starting from one corner. The
 *            walk * is confined to the box, thus there are at most 26 steps;
 *            for n>26 * the answer is 0. The algorithm performs a depth‑first
 *            search * exploring all walks and records th...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static int neighbors[27][6];
static unsigned char neighbor_cnt[27];
static seq_t walk_counts[27];
static bool initialized = false;

/* Recursive DFS that expands all self‑avoiding walks */
static void dfs(int pos, uint32_t mask, int step)
{
    walk_counts[step]++;                /* walk of length 'step' found */

    if (step == 26) return;             /* cannot take more steps */

    for (unsigned k = 0; k < neighbor_cnt[pos]; ++k) {
        int nxt = neighbors[pos][k];
        uint32_t bit = (uint32_t)1 << nxt;
        if (!(mask & bit)) {
            dfs(nxt, mask | bit, step + 1);
        }
    }
}

/* Helper that builds neighbor lists and precomputes walk counts */
static void init(void)
{
    /* Build adjacency lists for the 3×3×3 grid */
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                int idx = x * 9 + y * 3 + z;
                neighbor_cnt[idx] = 0;
                /* Six possible orthogonal moves */
                const int dirs[6][3] = {
                    {1, 0, 0}, {-1, 0, 0},
                    {0, 1, 0}, {0, -1, 0},
                    {0, 0, 1}, {0, 0, -1}
                };
                for (int d = 0; d < 6; ++d) {
                    int nx = x + dirs[d][0];
                    int ny = y + dirs[d][1];
                    int nz = z + dirs[d][2];
                    if (nx < 0 || nx > 2 || ny < 0 || ny > 2 || nz < 0 || nz > 2)
                        continue;
                    int nidx = nx * 9 + ny * 3 + nz;
                    neighbors[idx][neighbor_cnt[idx]++] = nidx;
                }
            }
        }
    }

    /* Start DFS from corner (0,0,0) which has index 0 */
    uint32_t start_mask = 1u << 0;
    dfs(0, start_mask, 0);

    initialized = true;
}

/* Main function requested by the grader */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n > 26U) return 0;             /* walk longer than space available */
    return walk_counts[(size_t)n];
}
