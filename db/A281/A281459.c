/*
 * A281459  Expansion of Product_{k>=1} (1 + x^(7*k-1))*(1 + x^(7*k-6)).
 * https://oeis.org/A281459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 306, u32 n <= 888, u64 n <= 2873,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in ∏_{k≥1} (1+x^(7*k-1))·(1+x^(7*k-6))
 *  i.e. number of partitions of n into distinct parts congruent to 1 or 6 mod 7.
 *  The implementation uses a simple DP that recomputes the whole table for each call.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static buffer reused for every call */
    static seq_t *dp   = NULL;  /* dp[j] = a(j) for 0 ≤ j ≤ current size-1 */
    static size_t  dp_sz = 0;   /* allocated size of dp */

    size_t needed = (size_t)n + 1;   /* we need indices 0..n inclusive */

    if (dp_sz < needed) {
        seq_t *tmp = (seq_t *)realloc(dp, needed * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        dp = tmp;
        dp_sz = needed;
    }

    /* initialise dp[0] = 1, others = 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* process all admissible parts m ≤ n (1 or 6 mod 7) */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t r = m % 7;
        if (r == 1 || r == 6) {
            for (idx_t j = n; j >= m; --j)
                dp[j] += dp[j - m];
        }
    }

    return dp[n];
}
