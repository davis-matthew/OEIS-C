/*
 * A269055  Number of 4Xn 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation: Dynamic programming over columns.  
 * Count the number of 4 × n arrays with entries 0,1,2 such that at most
 * one pair of horizontally, diagonally or antidiagonally adjacent
 * cells sums to 2.
 * The DP keeps the pattern of the previous column (81 possibilities)
 * and a flag indicating whether a pair summing to 2 has already
 * appeared (0 or 1).  The transition takes into account all of the
 * adjacency pairs that cross between two consecutive columns.
 * Complexity: 81² · n elementary operations (≈6.6×10⁹ ops for n=10⁵),
 * well below the 2‑second limit for n≈10⁴.
 */

#include <stdlib.h>
#include <stdbool.h>

#define ROWS 4
#define PATTS 81      /* 3⁴ */

static uint8_t pattern_digits[PATTS][ROWS];
static uint8_t paircnt[PATTS][PATTS];
static bool init_done = false;

/* Build the pattern tables once */
static void init_tables(void)
{
    if (init_done) return;
    /* Encode every pattern as 4 base‑3 digits */
    for (int p = 0; p < PATTS; ++p) {
        int tmp = p;
        for (int r = 0; r < ROWS; ++r) {
            pattern_digits[p][r] = tmp % 3;
            tmp /= 3;
        }
    }
    /* Count cross‑column pairs with sum 2 */
    for (int i = 0; i < PATTS; ++i) {
        for (int j = 0; j < PATTS; ++j) {
            unsigned cnt = 0;
            for (int r = 0; r < ROWS; ++r) {
                /* horizontal */
                if (pattern_digits[i][r] + pattern_digits[j][r] == 2) ++cnt;
                /* diagonal down‑right */
                if (r + 1 < ROWS &&
                    pattern_digits[i][r] + pattern_digits[j][r + 1] == 2) ++cnt;
                /* diagonal up‑right */
                if (r > 0 &&
                    pattern_digits[i][r] + pattern_digits[j][r - 1] == 2) ++cnt;
            }
            paircnt[i][j] = (uint8_t)cnt;
        }
    }
    init_done = true;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; a(0) is undefined */

    init_tables();

    /* DP arrays: dp[pattern][flag] */
    seq_t dp_cur[PATTS][2] = {0};
    seq_t dp_next[PATTS][2] = {0};

    /* First column: no pairs have been seen yet */
    for (int p = 0; p < PATTS; ++p) dp_cur[p][0] = 1;

    /* Process remaining columns */
    for (idx_t col = 2; col <= n; ++col) {
        /* Clear next DP */
        for (int q = 0; q < PATTS; ++q) {
            dp_next[q][0] = 0;
            dp_next[q][1] = 0;
        }
        for (int p = 0; p < PATTS; ++p) {
            seq_t cur0 = dp_cur[p][0];
            seq_t cur1 = dp_cur[p][1];
            if (cur0 == 0 && cur1 == 0) continue;
            for (int q = 0; q < PATTS; ++q) {
                uint8_t c = paircnt[p][q];
                if (c == 0) {
                    if (cur0) dp_next[q][0] += cur0;
                    if (cur1) dp_next[q][1] += cur1;
                } else if (c == 1) {
                    if (cur0) dp_next[q][1] += cur0;
                }   /* c>=2 : never allowed (would exceed the single‑pair rule) */
            }
        }
        /* Swap arrays */
        for (int q = 0; q < PATTS; ++q) {
            dp_cur[q][0] = dp_next[q][0];
            dp_cur[q][1] = dp_next[q][1];
        }
    }

    /* Sum all states after last column */
    seq_t total = 0;
    for (int p = 0; p < PATTS; ++p) {
        total += dp_cur[p][0];
        total += dp_cur[p][1];
    }
    return total;
}
