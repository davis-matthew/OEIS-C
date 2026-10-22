/*
 * A326443  Number of partitions of n into 8 squarefree parts.
 * https://oeis.org/A326443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326443: Number of partitions of n into 8 squarefree parts.
   Computation by dynamic programming over squarefree integers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    /* static state for memoisation */
    static idx_t max_n = 0;
    static seq_t *dp = NULL;          /* layout: dp[row * (max_n+1) + col]  */

    if (n > max_n) {
        /* free old table if any */
        if (dp) free(dp);

        /* allocate new table up to n */
        size_t cols = (size_t)(n + 1);
        size_t rows = 9;              /* k = 0..8 */
        dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
        if (!dp) return 0;            /* allocation failure */

        /* base case: exactly 0 parts sum to 0 */
        dp[0] = 1;

        /* sieve to find squarefree numbers up to n */
        bool *is_sf = (bool *)calloc(cols, sizeof(bool));
        if (!is_sf) { free(dp); return 0; }
        for (size_t i = 1; i < cols; ++i) is_sf[i] = true;

        idx_t limit = (idx_t)sqrt((double)n);
        for (idx_t p = 2; p <= limit; ++p) {
            if (is_sf[(size_t)p]) {                 /* p is prime */
                idx_t sq = p * p;
                for (idx_t m = sq; m <= n; m += sq)
                    is_sf[(size_t)m] = false;
            }
        }

        /* collect squarefree numbers */
        idx_t *sf = (idx_t *)malloc(cols * sizeof(idx_t));
        size_t sf_cnt = 0;
        for (size_t i = 1; i < cols; ++i)
            if (is_sf[i]) sf[sf_cnt++] = (idx_t)i;
        free(is_sf);

        /* DP over squarefree numbers as coins */
        for (size_t idx = 0; idx < sf_cnt; ++idx) {
            idx_t d = sf[idx];
            size_t start = (size_t)d;
            for (int k = 1; k <= 8; ++k) {
                size_t row   = (size_t)k * cols;
                size_t prev  = (size_t)(k - 1) * cols;
                for (size_t s = start; s < cols; ++s) {
                    dp[row + s] += dp[prev + (s - start)];
                }
            }
        }
        free(sf);
        max_n = n;
    }

    /* answer from memoised table */
    size_t cols = (size_t)(max_n + 1);
    return dp[(size_t)8 * cols + n];
}
