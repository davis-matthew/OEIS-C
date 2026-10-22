/*
 * A337021  The number of n-step self avoiding walks on a 3D cubic lattice
 *          confined inside a box of size 2x2x2 where the walk starts at the
 *          center of the box.
 * https://oeis.org/A337021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) counts the number of n‑step self‑avoiding walks in a 3×3×3
 *            cubic lattice starting at the centre vertex (1,1,1). For n>26
 *            the value is 0 because a 2×2×2 box contains only 27 vertices.
 *            The function enumerates all walks by exhaustive depth‑first
 *            search and memoises the results
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define VERTICES 27

/* adjacency lists for each vertex in a 3×3×3 grid */
static unsigned char neigh[VERTICES][6];
static unsigned char neigh_cnt[VERTICES];

/* table of walk counts for lengths 0 … 26 */
static seq_t walk_counts[27] = {0};
static bool precomputed = false;

/* recursive DFS exploring simple walks */
static void dfs(int v, uint32_t mask, int depth)
{
    /* if maximum depth reached, stop */
    if (depth == 26)
        return;

    unsigned char cnt = neigh_cnt[v];
    for (int i = 0; i < cnt; ++i) {
        int u = neigh[v][i];
        uint32_t bit = 1u << u;
        if (mask & bit)
            continue;               /* vertex already visited */
        walk_counts[depth + 1]++;    /* new walk of length depth+1 */
        dfs(u, mask | bit, depth + 1);
    }
}

/* build adjacency lists and pre‑compute all walk counts once */
static void precompute(void)
{
    /* build adjacency for the 3×3×3 grid */
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z) {
                int idx = x * 9 + y * 3 + z;
                int c = 0;
                if (x > 0) neigh[idx][c++] = (x - 1) * 9 + y * 3 + z;
                if (x < 2) neigh[idx][c++] = (x + 1) * 9 + y * 3 + z;
                if (y > 0) neigh[idx][c++] = x * 9 + (y - 1) * 3 + z;
                if (y < 2) neigh[idx][c++] = x * 9 + (y + 1) * 3 + z;
                if (z > 0) neigh[idx][c++] = x * 9 + y * 3 + (z - 1);
                if (z < 2) neigh[idx][c++] = x * 9 + y * 3 + (z + 1);
                neigh_cnt[idx] = (unsigned char)c;
            }

    /* starting vertex is the centre (1,1,1) */
    const int start = 13;
    walk_counts[0] = 1;                 /* empty walk */
    dfs(start, 1u << start, 0);         /* explore all simple walks */
    precomputed = true;
}

/* required public function */
seq_t a(idx_t n)
{
    if (!precomputed)
        precompute();

    if (n > (idx_t)26)
        return 0;

    return walk_counts[(size_t)n];
}
