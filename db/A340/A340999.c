/*
 * A340999  Number of partitions of n into 8 distinct nonzero squares.
 * https://oeis.org/A340999
 *
 * Offset:    204    (a(204) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 8 distinct non‑zero
 *            squares
 * Range:     u8 n <= 255, u16 n <= 303, u32 n <= 303, u64 n <= 303,
 *            u128 n <= 303, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer square root for unsigned 64‑bit values.  */
static idx_t isqrt(idx_t n) {
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* DP arrays for counts of k distinct squares summing to s.  */
static seq_t *dp[9] = {0};           /* dp[0..8][0..max_n] */
static seq_t *answer_cache = NULL;   /* pointer to dp[8] after each rebuild */
static idx_t  max_cached = 0;        /* highest n for which we have computed */

/* Recompute DP for all sums up to 'max_n'.  */
static void recompute(idx_t max_n) {
    /* Deallocate old arrays.  */
    for (int k = 0; k <= 8; ++k) {
        free(dp[k]);
        dp[k] = NULL;
    }

    /* Allocate new arrays, zeroed.  */
    for (int k = 0; k <= 8; ++k) {
        dp[k] = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
        if (!dp[k]) exit(EXIT_FAILURE);   /* allocation failure aborts */
    }

    dp[0][0] = (seq_t)1;   /* one way to pick 0 squares with sum 0 */

    /* Loop over all possible square values up to sqrt(max_n).  */
    idx_t limit = isqrt(max_n);
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t value = (seq_t)i * (seq_t)i;          /* i² */
        idx_t val_idx = (idx_t)value;              /* for array indexing */

        for (int k = 8; k >= 1; --k) {
            for (idx_t s = max_n; s >= val_idx; --s) {
                dp[k][s] += dp[k-1][s - val_idx];
            }
        }
    }

    answer_cache = dp[8];
    max_cached = max_n;
}

seq_t a(idx_t n) {
    /* OEIS offset is 204; values below this are undefined.  */
    if (n < 204) return (seq_t)0;

    if (n > max_cached || answer_cache == NULL) {
        recompute(n);
    }

    return answer_cache[n];
}
