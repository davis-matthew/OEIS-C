/*
 * A280951  Expansion of Product_{k>=0} 1/(1 - x^(2*k*(k+1)+1)).
 * https://oeis.org/A280951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into centered square numbers (OEIS
 *            A280951). The function implements an unbounded‑knapsack DP that
 *            computes all partition counts up to the largest argument seen
 *            (memoised in a static array)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of partition counts up to the greatest n seen. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Expand the cache if necessary. */
    if (n >= dp_size)
    {
        idx_t new_size = n + 1;

        /* Allocate a new array of seq_t, constructed if C++. */
#ifdef __cplusplus
        seq_t *new_dp = new seq_t[new_size];
#else
        seq_t *new_dp = (seq_t*)malloc((size_t)new_size * sizeof(seq_t));
#endif
        if (!new_dp)
            return (seq_t)0;   /* out of memory; 0 denotes error */

        /* Initialise all elements to zero. */
        for (idx_t i = 0; i < new_size; ++i)
            new_dp[i] = 0;
        new_dp[0] = 1;          /* one way to make 0: use no parts */

        /* Generate all centred square numbers <= n
           and perform the classic “unbounded knapsack” DP update. */
        for (idx_t k = 0; ; ++k)
        {
            idx_t p = 2 * k * (k + 1) + 1;   /* 2*k(k+1)+1 */
            if (p > n)
                break;

            for (idx_t i = p; i <= n; ++i)
                new_dp[i] += new_dp[i - p];
        }

        /* Replace the old cache. */
        if (dp != NULL)
        {
#ifdef __cplusplus
            delete[] dp;
#else
            free(dp);
#endif
        }
        dp      = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}
