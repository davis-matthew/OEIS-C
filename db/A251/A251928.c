/*
 * A251928  Number of length n+2 0..1 arrays with the sum of the maximum minus
 *          the median of adjacent triples multiplied by some arrangement of
 *          +-1 equal to zero.
 * https://oeis.org/A251928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 6*a(n-3) - 4*a(n-4), * a(1)=5,
 *            a(2)=10, a(3)=18, a(4)=34
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity in elements   */
    static idx_t cache_len = 0;   /* number of values actually stored */

    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    /* Initialise the cache with the base values if it is still empty. */
    if (cache_cap < 4)
    {
        /* Allocate space for the first four terms. */
#ifdef __cplusplus
        cache = new seq_t[4];
#else
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
#endif
        if (!cache) return 0;      /* allocation failed */

        cache_cap = 4;
        cache_len = 4;
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)18;
        cache[3] = (seq_t)34;
    }

    /* If the requested term has already been computed, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache if necessary.  Use a doubling strategy. */
    if (cache_cap < n)
    {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < n) new_cap = n;

#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failed */
        cache = new_cache;
#endif

        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_len; i < n; ++i)
    {
        seq_t t1 = 4 * cache[i - 1];
        seq_t t2 = 6 * cache[i - 2];
        seq_t t3 = 6 * cache[i - 3];
        seq_t t4 = 4 * cache[i - 4];
        cache[i] = t1 - t2 + t3 - t4;
    }

    cache_len = n;
    return cache[n - 1];
}
