/*
 * A341000  Number of partitions of n into 9 distinct nonzero squares.
 * https://oeis.org/A341000
 *
 * Offset:    285    (a(285) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 9 distinct nonzero squares
 * Range:     u8 none, u16 n <= 3428, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer square root – integer part of sqrt(n) */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) { low = 1; continue; }
        if (mid <= n / mid) {
            if ((mid + 1) > n / (mid + 1)) return mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high; /* never reached */
}

/* DP tables: dp[k][s] = number of ways to write s as sum of k distinct squares */
static seq_t *dp[10] = {NULL};
static idx_t computed_max_n = 0;
static bool ready = false;

/* Allocate (or reallocate) DP tables up to max_n */
static void _ensure_dp(idx_t max_n)
{
    if (ready && computed_max_n >= max_n) return;

    /* free old tables */
    for (int k = 0; k < 10; ++k) {
        if (dp[k]) free(dp[k]);
        dp[k] = NULL;
    }

    /* allocate new tables – cast needed for C++ */
    for (int k = 0; k < 10; ++k) {
        dp[k] = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
        if (!dp[k]) abort(); /* memory error */
    }

    dp[0][0] = (seq_t)1;  /* empty sum */

    idx_t limit = _isqrt_u64(max_n);
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;
        idx_t sq_i = (idx_t)sq; /* fits in idx_t for n <= 10000 */
        for (int k = 9; k >= 1; --k) {
            for (idx_t s = max_n; s >= sq_i; --s) {
                dp[k][s] += dp[k-1][s - sq_i];
            }
        }
    }

    computed_max_n = max_n;
    ready = true;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 285) return (seq_t)0;   /* undefined below the offset */
    _ensure_dp(n);
    return dp[9][n];
}
