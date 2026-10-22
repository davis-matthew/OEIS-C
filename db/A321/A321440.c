/*
 * A321440  Number of partitions of n into consecutive parts, all singletons
 *          except the largest.
 * https://oeis.org/A321440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into consecutive parts, all
 *            singletons except the largest. Computation by enumerating
 *            triples (i,j,t) where i <= j and t >= 1
 * Range:     u8 n <= 3428, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, free */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* number of elements in dp (dp_size == max_n + 1) */

    /* recompute up to n if needed */
    if (n >= dp_size) {
        seq_t *new_dp = (seq_t *)calloc((size_t)(n + 1U), sizeof(seq_t));
        if (!new_dp) {
            /* allocation failure – abort (no error handling required by spec) */
            abort();
        }

        /* base case: the empty partition of 0 */
        new_dp[0] = 1;

        /* enumerate all triples (i,j,t) that produce a partition of n
           and increment the corresponding dp entry. */
        for (idx_t j = 1; j <= n; ++j) {
            /* case i == j : base = 0 (empty sum over k=i..j-1) */
            idx_t max_t = (n - 0) / j;
            for (idx_t t = 1; t <= max_t; ++t) {
                new_dp[t * j] += 1;
            }

            /* cases i < j : sum over k=i..j-1 is positive */
            idx_t sum = 0;
            for (idx_t i = j - 1; i; --i) {
                sum += i;                 /* now sum = Σ_{k=i}^{j-1} k */
                if (sum > n) continue;    /* base already exceeds n */
                idx_t max_t2 = (n - sum) / j;
                for (idx_t t = 1; t <= max_t2; ++t) {
                    new_dp[sum + t * j] += 1;
                }
            }
        }

        /* replace old cache */
        free(dp);
        dp = new_dp;
        dp_size = n + 1U;
    }

    return dp[n];
}
