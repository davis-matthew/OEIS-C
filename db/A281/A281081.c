/*
 * A281081  Expansion of Product_{k>=0} (1 + x^(3*k*(k+1)/2+1)).
 * https://oeis.org/A281081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct centered
 *            triangular numbers (a generating function product_{k>=0} (1 +
 *            x^(3*k*(k+1)/2+1))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of all values up to the largest n requested so far */
    static seq_t *cache = NULL;      /* array holding a[0 .. cached_max] */
    static idx_t cached_max = (idx_t)-1;   /* maximum index stored in cache */

    /* If we already have a(n) cached, return it */
    if (cache != NULL && n <= cached_max)
        return cache[n];

    /* Recompute from scratch for all indices up to n */
    idx_t limit = n;

    /* Allocate array for dynamic programming */
    seq_t *dp = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failure */

    /* Initialize dp array */
    for (idx_t i = 0; i <= limit; ++i)
        dp[i] = 0;
    dp[0] = 1;   /* empty partition */

    /* Process all centered triangular numbers <= limit */
    for (idx_t k = 0;; ++k) {
        /* part = 3*k*(k+1)/2 + 1 */
        seq_t part = (seq_t)3 * k * (k + 1) / 2 + 1;
        if (part > (seq_t)limit)
            break;      /* no more parts needed */

        /* Subset‑sum DP: each part can be used at most once */
        for (idx_t i = limit; i >= (idx_t)part; --i)
            dp[i] = dp[i] + dp[i - (idx_t)part];
    }

    /* Replace previous cache with newly computed values */
    free(cache);
    cache = dp;
    cached_max = limit;

    return cache[n];
}
