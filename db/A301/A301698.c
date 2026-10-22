/*
 * A301698  Partial sums of A301697.
 * https://oeis.org/A301698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 4*a(n-4) * + 4*a(n-5) -
 *            3*a(n-6) + a(n-7) for n > 6 * Initial values: a(0)=1, a(1)=6,
 *            a(2)=16, a(3)=32, a(4)=54, * a(5)=81, a(6)=113
 * Range:     u8 n <= 9, u16 n <= 156, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated capacity */
    static idx_t last_index = 0;   /* highest index computed */

    /* initialize cache with first 7 terms */
    if (!cache) {
#if defined(__cplusplus)
        cache = new seq_t[7];
#else
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0; /* unlikely, error return */

        cache[0] = 1;   /* a(0) */
        cache[1] = 6;   /* a(1) */
        cache[2] = 16;  /* a(2) */
        cache[3] = 32;  /* a(3) */
        cache[4] = 54;  /* a(4) */
        cache[5] = 81;  /* a(5) */
        cache[6] = 113; /* a(6) */

        cache_size = 7;
        last_index = 6;
    }

    /* fast return if already computed */
    if (n < cache_size)
        return cache[n];

    /* grow the cache to accommodate up to n */
    idx_t new_size = n + 1;
#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_size];
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0; /* allocation failure */
    cache = tmp;
#endif
    cache_size = new_size;

    /* compute missing terms using the recurrence */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        /* group positive and negative terms separately to avoid
         * intermediate underflow on unsigned types */
        seq_t pos = 3 * cache[i - 1];
        pos += 4 * cache[i - 3];
        pos += 4 * cache[i - 5];
        pos += cache[i - 7];

        seq_t neg = 4 * cache[i - 2];
        neg += 4 * cache[i - 4];
        neg += 3 * cache[i - 6];

        cache[i] = pos - neg;
    }
    last_index = n;

    return cache[n];
}
