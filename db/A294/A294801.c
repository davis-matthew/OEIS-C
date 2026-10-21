/*
 * A294801  Number of permutations of [n] avoiding {4231, 1324, 2341}.
 * https://oeis.org/A294801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 41*a(n-2) + 89*a(n-3) - 110*a(n-4) +
 *            77*a(n-5) - 28*a(n-6) + 4*a(n-7) a(0) = 1, a(1) = 1, a(2) = 2,
 *            a(3) = 6, a(4) = 21, a(5) = 73, a(6) = 239
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef SEQ_H
#define SEQ_H
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <algorithm>
#endif

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;
    static idx_t   cache_size = 0;
    static idx_t   last_computed = 0;
    static bool    initialized = false;

    /* initial values (a[0]..a[6]) */
    const seq_t init_vals[7] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
                                 (seq_t)21, (seq_t)73, (seq_t)239 };

    /* initialise on first call */
    if (!initialized) {
        /* allocate array for indices 0..6 */
        idx_t init_size = 7;
#ifdef __cplusplus
        cache = new seq_t[init_size];
        for (idx_t i = 0; i < init_size; ++i)
            cache[i] = init_vals[i];
#else
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        for (idx_t i = 0; i < init_size; ++i)
            cache[i] = init_vals[i];
#endif
        cache_size = init_size;
        last_computed = 6;
        initialized = true;
    }

    /* if requested value already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* grow storage if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
        /* copy existing values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        /* zero new elements */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        delete[] cache;
        cache = new_cache;
        cache_size = new_size;
#else
        new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* zero new elements */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
#endif
    }

    /* compute missing values up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t pos = (seq_t)10 * cache[k-1];
        pos += (seq_t)89 * cache[k-3];
        pos += (seq_t)77 * cache[k-5];
        pos += (seq_t)4  * cache[k-7];

        seq_t neg = (seq_t)41 * cache[k-2];
        neg += (seq_t)110 * cache[k-4];
        neg += (seq_t)28  * cache[k-6];

        /* the recurrence guarantees pos >= neg for all k */
        cache[k] = pos - neg;
    }

    last_computed = n;
    return cache[n];
}
#endif /* SEQ_H */
