/*
 * A335806  The number of n-step self avoiding walks on a 3D cubic lattice
 *          confined inside a box of size 2x2x2 where the walk starts at the
 *          middle of the box's edge.
 * https://oeis.org/A335806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   a(n) = number of n‑step self‑avoiding walks inside a 2×2×2 cube
 *            starting from the midpoint of an edge. Computed by exhaustive
 *            DFS up to n = 26; for n > 26 return 0
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_N 26         /* walks longer than 26 cannot stay inside the cube */

/* Result table, initialized on first use. */
static seq_t result[MAX_N + 1] = {0};
static bool initialized = false;

/* Lattice step vectors. */
static const int DX[6] = { 1, -1, 0, 0, 0, 0 };
static const int DY[6] = { 0,  0, 1, -1, 0, 0 };
static const int DZ[6] = { 0,  0, 0, 0, 1, -1 };

/* Depth‑first search that counts walks of each length. */
static void dfs(int pos, uint32_t mask, int depth)
{
    /* Count the walk of current length. */
    result[depth]++;

    if (depth == MAX_N)
        return;

    /* Convert linear index to 3‑D coordinates. */
    int x = pos / 9;
    int y = (pos % 9) / 3;
    int z = pos % 3;

    for (int dir = 0; dir < 6; ++dir) {
        int nx = x + DX[dir];
        int ny = y + DY[dir];
        int nz = z + DZ[dir];
        if (nx < 0 || nx > 2 || ny < 0 || ny > 2 || nz < 0 || nz > 2)
            continue;
        int npos = nx * 9 + ny * 3 + nz;
        uint32_t nbit = 1u << npos;
        if (mask & nbit)          /* already visited */
            continue;
        dfs(npos, mask | nbit, depth + 1);
    }
}

/* Initialise the table once. */
static void init_counts(void)
{
    const int start = 1 * 9 + 0 * 3 + 0; /* (1,0,0): middle of an edge */
    dfs(start, 1u << start, 0);
}

seq_t a(idx_t n)
{
    if (n > MAX_N)           /* walks too long to fit in the cube */
        return 0;

    if (!initialized) {
        init_counts();
        initialized = true;
    }
    return result[(size_t)n];
}
