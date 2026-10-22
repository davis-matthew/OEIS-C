/*
 * A395927  a(n) is the number of different possible words of length n that
 *          can be found in a Boggle grid.
 * https://oeis.org/A395927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, walk
 * Formula:   a(n) = number of self‑avoiding walks of length n on a 4×4
 *            king’s‑move board (0 for n > 16, a(1)=16)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* for uint32_t */
#include <stdlib.h>   /* for malloc, free */

/* adjacency lists for a 4×4 grid with king moves */
static const int neighbors[16][8] = {
    { 1,  4,  5, -1, -1, -1, -1, -1},   /* 0 */
    { 0,  2,  4,  5,  6, -1, -1, -1},   /* 1 */
    { 1,  3,  5,  6,  7, -1, -1, -1},   /* 2 */
    { 2,  6,  7, -1, -1, -1, -1, -1},   /* 3 */
    { 0,  1,  5,  8,  9, -1, -1, -1},   /* 4 */
    { 0,  1,  2,  4,  6,  8,  9, 10},    /* 5 */
    { 1,  2,  3,  5,  7,  9, 10, 11},    /* 6 */
    { 2,  3,  6, 10, 11, -1, -1, -1},    /* 7 */
    { 4,  5,  9, 12, 13, -1, -1, -1},    /* 8 */
    { 4,  5,  6,  8, 10, 12, 13, 14},    /* 9 */
    { 5,  6,  7,  9, 11, 13, 14, 15},    /*10*/
    { 6,  7, 10, 14, 15, -1, -1, -1},    /*11*/
    { 8,  9, 13, -1, -1, -1, -1, -1},    /*12*/
    { 8,  9, 10, 12, 14, -1, -1, -1},    /*13*/
    { 9, 10, 11, 13, 15, -1, -1, -1},    /*14*/
    {10, 11, 14, -1, -1, -1, -1, -1}     /*15*/
};
static const int neigh_cnt[16] = {3,5,5,3,5,8,8,5,5,8,8,5,3,5,5,3};

/* flattened DP index: mask in 16 bits, pos in 4 bits */
#define IDX(mask, pos) (((mask) << 4) + (pos))

/* DP table: number of walks ending at pos having visited cells in mask */
static seq_t dp[(1U << 16) * 16];
static seq_t values[17] = {0};
static int init_done = 0;

/* compute all values up to n = 16 once */
static void compute(void)
{
    /* starting walks of length 1 */
    for (int i = 0; i < 16; ++i)
        dp[IDX(1U << i, i)] = 1;

    const unsigned int FULL = (1U << 16) - 1;
    for (unsigned int mask = 1; mask <= FULL; ++mask) {
        for (int pos = 0; pos < 16; ++pos) {
            if (!(mask & (1U << pos))) continue;           /* pos not in mask */
            seq_t cur = dp[IDX(mask, pos)];
            if (!cur) continue;
            int nb_cnt = neigh_cnt[pos];
            const int *nbrs = neighbors[pos];
            for (int k = 0; k < nb_cnt; ++k) {
                int nb = nbrs[k];
                if (mask & (1U << nb)) continue;           /* already visited */
                dp[IDX(mask | (1U << nb), nb)] += cur;
            }
        }
    }

    /* accumulate counts for each walk length */
    for (unsigned int mask = 1; mask <= FULL; ++mask) {
        unsigned int len = __builtin_popcount(mask);
        if (len > 16) continue;
        for (int pos = 0; pos < 16; ++pos) {
            if (mask & (1U << pos))
                values[len] += dp[IDX(mask, pos)];
        }
    }

    init_done = 1;
}

/* public accessor */
seq_t a(idx_t n)
{
    if (!init_done) compute();
    if (n < 1 || n > 16) return 0;
    return values[(size_t)n];
}
