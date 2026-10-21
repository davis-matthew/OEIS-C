/*
 * A282583  Number of compositions (ordered partitions) of n into
 *          quarter-squares (A002620).
 * https://oeis.org/A282583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0,
   a(n)=∑_{k≥2, floor(k^2/4)≤n} a(n−floor(k^2/4)).  */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed values. */
    static seq_t *dp     = NULL;
    static idx_t  size   = 0;

    /* Grow the cache if necessary. */
    if (size <= n + 1) {
        idx_t new_size = n + 1;
        dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (dp == NULL) return 0;          /* Out of memory – unlikely here. */
        if (size > 0) {
            memcpy(dp, dp, size * sizeof(seq_t)); /* copy old values */
        }
        /* Initialize new entries to 0. */
        memset(dp + size, 0, (new_size - size) * sizeof(seq_t));
        size = new_size;
        dp[0] = 1;                         /* base case */
    }

    /* Compute values up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        if (dp[i] != 0) continue;          /* already computed */
        seq_t sum = 0;
        for (idx_t k = 2;; ++k) {
            idx_t m = (k * k) / 4;          /* floor(k^2/4) */
            if (m > i) break;
            sum += dp[i - m];
        }
        dp[i] = sum;
    }

    return dp[n];
}
