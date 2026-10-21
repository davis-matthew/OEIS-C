/*
 * A278154  Number of nX5 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) (1,-1) or (1,0), with upper left element zero.
 * https://oeis.org/A278154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A278154: number of n×5 binary arrays with the local
   constraint that each cell has at least one equal and one unequal
   neighbor among the offsets (-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,-1),(1,0),
   with the top‑left element fixed to 0.
   The algorithm uses dynamic programming over rows and precomputed valid
   transitions. */

#include <stdbool.h>
#include <stdint.h>

#define WIDTH 5                               /* columns in the array */
#define PATTERS (1U << WIDTH)                 /* 2^5 = 32 possible rows    */
#define PREV2_SZ (PATTERS + 1)                /* +1 for sentinel (no row)   */
static const int SENTINEL = PATTERS;          /* index representing missing row */

/* test whether a given row (prev1) satisfies the constraint,
   given the preceding row (prev2) and the following row (curr)
   if has_curr is true; otherwise the following row is absent   */
static bool row_valid(int prev2_idx, int prev1_idx, bool has_curr, int curr_idx)
{
    for (int col = 0; col < WIDTH; ++col) {
        int v = (prev1_idx >> col) & 1;
        bool has_eq = false, has_neq = false;

        /* offset (-1,-1) */
        if (prev2_idx < PATTERS && col > 0) {
            int nb = (prev2_idx >> (col - 1)) & 1;
            if (nb == v) has_eq = true; else has_neq = true;
        }
        /* offset (-1,0) */
        if (prev2_idx < PATTERS) {
            int nb = (prev2_idx >> col) & 1;
            if (nb == v) has_eq = true; else has_neq = true;
        }
        /* offset (-1,1) */
        if (prev2_idx < PATTERS && col < WIDTH - 1) {
            int nb = (prev2_idx >> (col + 1)) & 1;
            if (nb == v) has_eq = true; else has_neq = true;
        }
        /* offset (0,-1) */
        if (col > 0) {
            int nb = (prev1_idx >> (col - 1)) & 1;
            if (nb == v) has_eq = true; else has_neq = true;
        }
        /* offset (0,1) */
        if (col < WIDTH - 1) {
            int nb = (prev1_idx >> (col + 1)) & 1;
            if (nb == v) has_eq = true; else has_neq = true;
        }
        /* offset (1,-1) */
        if (has_curr && col > 0) {
            int nb = (curr_idx >> (col - 1)) & 1;
            if (nb == v) has_eq = true; else has_neq = true;
        }
        /* offset (1,0) */
        if (has_curr) {
            int nb = (curr_idx >> col) & 1;
            if (nb == v) has_eq = true; else has_neq = true;
        }

        if (!has_eq || !has_neq) return false;
    }
    return true;
}

/* The main function requested by the exercise */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n=0, safeguard */

    /* Precompute transition masks only once */
    static bool initialized = false;
    static uint32_t mid_mask[PREV2_SZ][PATTERS];
    static bool   last_valid[PREV2_SZ][PATTERS];

    if (!initialized) {
        initialized = true;
        for (int p2 = 0; p2 < PREV2_SZ; ++p2) {
            for (int p1 = 0; p1 < PATTERS; ++p1) {
                uint32_t mask = 0;
                for (int pc = 0; pc < PATTERS; ++pc) {
                    if (row_valid(p2, p1, true, pc))
                        mask |= (1U << pc);
                }
                mid_mask[p2][p1] = mask;
                last_valid[p2][p1] = row_valid(p2, p1, false, 0);
            }
        }
    }

    /* Dynamic programming arrays */
    static seq_t dp[PREV2_SZ][PATTERS];
    static seq_t ndp[PREV2_SZ][PATTERS];

    /* initialize dp for the first row (row index 0) */
    for (int i = 0; i < PREV2_SZ; ++i)
        for (int j = 0; j < PATTERS; ++j)
            dp[i][j] = 0;
    for (int p = 0; p < PATTERS; ++p)
        if ((p & 1) == 0)          /* top-left element must be 0 */
            dp[SENTINEL][p] = 1;

    /* iterate over remaining rows */
    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i < PREV2_SZ; ++i)
            for (int j = 0; j < PATTERS; ++j)
                ndp[i][j] = 0;

        for (int p2 = 0; p2 < PREV2_SZ; ++p2) {
            for (int p1 = 0; p1 < PATTERS; ++p1) {
                seq_t cnt = dp[p2][p1];
                if (cnt == 0) continue;
                uint32_t mask = mid_mask[p2][p1];
                for (int pc = 0; pc < PATTERS; ++pc) {
                    if (mask & (1U << pc))
                        ndp[p1][pc] += cnt;
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (int i = 0; i < PREV2_SZ; ++i)
            for (int j = 0; j < PATTERS; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum over final states, validating the last row */
    seq_t result = 0;
    for (int p2 = 0; p2 < PREV2_SZ; ++p2) {
        for (int p1 = 0; p1 < PATTERS; ++p1) {
            seq_t cnt = dp[p2][p1];
            if (cnt == 0) continue;
            if (last_valid[p2][p1]) result += cnt;
        }
    }
    return result;
}
