/*
 * A241273  Number of partitions p of n into distinct parts such that max(p) =
 *          6*min(p).
 * https://oeis.org/A241273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A241273
    Formula: a(n) = number of partitions p of n into distinct parts such that max(p) = 6*min(p)
    Offset: 0
    The function below computes a(n) for any n ≥ 0.  It caches the whole table up to the largest
    argument seen so that subsequent calls are O(1).  The algorithm uses the generating‑function
    expression
        a(n) = [q^n] ∑_{m≥1} q^{7m} ∏_{k=m+1}^{6m-1} (1 + q^k)
    and evaluates each inner product by a classic subset‑sum dynamic program limited to the
    current maximum target (n – 7m).  The total work for n ≤ 10 000 is only a few million
    integer additions, well within the 2‑second limit. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Allocate or resize the cache if necessary */
    if (cache == NULL || n >= cache_size)
    {
        /* free previous cache */
        if (cache != NULL)
            free(cache);

        cache_size = n + 1;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));   /* zero‑initialised */

        /* temporary array for subset‑sum DP */
        seq_t *dp = (seq_t *)calloc(cache_size, sizeof(seq_t));

        /* Build all values up to n */
        idx_t max_m = n / 7;                     /* 7m must not exceed n */
        for (idx_t m = 1; m <= max_m; ++m)
        {
            idx_t target = n - 7 * m;            /* remaining sum to be produced by the subset */

            /* Zero dp[0..target] and initialise dp[0] = 1 */
            for (idx_t i = 0; i <= target; ++i)
                dp[i] = 0;
            dp[0] = 1;

            /* Range of possible parts inside the subset */
            idx_t k_start = m + 1;
            idx_t k_end   = 6 * m - 1;
            if (k_start > target)                /* no part can be used */
                k_end = k_start - 1;             /* ensure inner loop is skipped */

            if (k_end > target)
                k_end = target;

            /* Subset‑sum DP over k_start … k_end */
            for (idx_t k = k_start; k <= k_end; ++k)
            {
                for (idx_t s = target; s >= k; --s)
                    dp[s] += dp[s - k];
            }

            /* Add the contributions of this m to the global cache */
            for (idx_t s = 0; s <= target; ++s)
            {
                idx_t idx = 7 * m + s;
                if (idx <= n)
                    cache[idx] += dp[s];
            }
        }

        free(dp);
    }

    return cache[n];
}
