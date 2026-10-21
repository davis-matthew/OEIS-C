/*
 * A269217  Number of 4 X n 0..3 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling three exactly once.
 * https://oeis.org/A269217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 4×n 0..3 arrays, exactly one adjacent pair (horiz., diag., anti‑diag.) sums to 3.
   The program uses dynamic programming over the 4‑row column patterns
   (there are 4⁴ = 256 patterns).  For each column we keep the number of
   partial arrays that end in that column with 0 or 1 such pairs so far.
   Transitions between adjacent columns are pre‑computed blocking all
   pairs that would create more than one occurrence.  This yields the
   exact count for any n ≥ 1, and the implementation is valid for any
   unsigned integer seq_t (8,16,32,64,128 bits or arbitrary precision).   */

#include <stdbool.h>
#include <stdint.h>

/* Number of possible column patterns: 4⁴. */
#define COLS 256

/* Transition tables: from a column to a next column resulting in 0 or 1 new pair. */
static uint16_t zero_next[COLS][COLS];
static uint8_t  zero_cnt[COLS];
static uint16_t one_next[COLS][COLS];
static uint8_t  one_cnt[COLS];
static bool initialized = false;

/* Pre‑compute all transitions.  zero_cnt/one_cnt tell how many entries
   in zero_next/one_next are valid for each source column. */
static void build_lists(void)
{
    for (int c1 = 0; c1 < COLS; ++c1) {
        zero_cnt[c1] = 0;
        one_cnt[c1]  = 0;
        for (int c2 = 0; c2 < COLS; ++c2) {
            int sum = 0;
            for (int r = 0; r < 4; ++r) {
                int v1 = (c1 >> (2 * r)) & 3;
                int v2 = (c2 >> (2 * r)) & 3;
                if (v1 + v2 == 3) ++sum;          /* horizontal */

                if (r < 3) {                      /* anti‑diagonal: (c1 r) – (c2 r+1) */
                    int v2_next = (c2 >> (2 * (r + 1))) & 3;
                    if (v1 + v2_next == 3) ++sum;
                }
                if (r > 0) {                      /* diagonal: (c1 r) – (c2 r‑1) */
                    int v2_prev = (c2 >> (2 * (r - 1))) & 3;
                    if (v1 + v2_prev == 3) ++sum;
                }
            }
            if (sum == 0) {
                zero_next[c1][zero_cnt[c1]++] = (uint16_t)c2;
            } else if (sum == 1) {
                one_next[c1][one_cnt[c1]++]  = (uint16_t)c2;
            }
            /* sum > 1 transitions are discarded */
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 0;  /* a(1) = 0 by definition */

    if (!initialized) {
        initialized = true;
        build_lists();
    }

    seq_t dp0[COLS], dp1[COLS], ndp0[COLS], ndp1[COLS];

    /* Initialize for the first column: no paired sums yet. */
    for (int i = 0; i < COLS; ++i) {
        dp0[i] = 1;
        dp1[i] = 0;
    }

    /* Process subsequent columns. */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < COLS; ++i) {
            ndp0[i] = 0;
            ndp1[i] = 0;
        }

        for (int c = 0; c < COLS; ++c) {
            /* Transitions adding 0 new pairs. */
            for (int k = 0; k < zero_cnt[c]; ++k) {
                int nb = zero_next[c][k];
                ndp0[nb] += dp0[c];
                ndp1[nb] += dp1[c];
            }
            /* Transitions adding 1 new pair – only from dp0 (previously 0). */
            for (int k = 0; k < one_cnt[c]; ++k) {
                int nb = one_next[c][k];
                ndp1[nb] += dp0[c];
            }
        }

        /* Move next state to current. */
        for (int i = 0; i < COLS; ++i) {
            dp0[i] = ndp0[i];
            dp1[i] = ndp1[i];
        }
    }

    /* Sum all configurations that have exactly one sum‑to‑3 pair. */
    seq_t result = 0;
    for (int i = 0; i < COLS; ++i) result += dp1[i];
    return result;
}
