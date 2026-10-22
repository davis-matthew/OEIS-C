/*
 * A360182  Number of partitions of [n] where each block size occurs at most
 *          twice.
 * https://oeis.org/A360182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of [n] where each block size occurs
 *            at most twice
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a recurrence derived from the exponential generating function. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* current size of cache (elements per cache[0..cache_size-1]) */
    static idx_t max_computed = 0; /* maximum n for which cache is valid */

    if (n < max_computed)  /* cached value available */
        return cache[n];

    /* Allocate new cache and temporary arrays of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_cache) return 0;        /* in case of allocation failure */
    memset(new_cache, 0, sz * sizeof(seq_t));
    new_cache[0] = 1;

    seq_t *dp   = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *bin  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp || !bin) {
        free(new_cache); free(dp); free(bin);
        return 0;
    }
    memset(dp, 0, sz * sizeof(seq_t));
    dp[0] = 1;

    /* Main DP: process block sizes k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        /* compute bin[t] = C(t,k) for t = k..n */
        for (idx_t t = 0; t < k; ++t) bin[t] = 0;
        for (idx_t t = k; t <= n; ++t) {
            if (t == k) bin[t] = 1;
            else bin[t] = bin[t - 1] * (seq_t)t / (seq_t)(t - k);
        }

        /* update dp array in decreasing order to use previous values */
        for (idx_t s = n; s >= k; --s) {
            seq_t term1 = bin[s] * dp[s - k];
            seq_t term2 = 0;
            if (s >= 2 * k) {
                seq_t prod = bin[s] * bin[s - k];
                prod = prod / 2;     /* division is exact */
                term2 = prod * dp[s - 2 * k];
            }
            dp[s] = dp[s] + term1 + term2;
        }
    }

    /* copy results to the cache */
    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = dp[i];

    /* free temporary arrays */
    free(dp);
    free(bin);

    /* replace old cache if any */
    free(cache);
    cache = new_cache;
    cache_size = sz;
    max_computed = n;

    return cache[n];
}
