/*
 * A299099  Number of (n + 1, n + 2)-core partitions with odd parts and
 *          corresponding order ideals confined to the two outermost diagonals
 *          of P_{n + 1, n + 2}.
 * https://oeis.org/A299099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5); initial
 *            values: a(0)=1, a(1)=2, a(2)=4, a(3)=7, a(4)=15
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of values currently stored */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* number of terms we need (indices 0 .. n inclusive) */
    size_t needed = (size_t)n + 1;

    /* grow the cache if necessary */
    if (needed > cache_cap) {
        /* choose a new capacity (double until large enough) */
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap < needed)
            new_cap <<= 1;

        /* allocate a new array of that capacity */
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

        /* copy the old values into the new array */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* free the old array if it existed */
        if (cache) {
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute new values up to the required index */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;          /* a(0) */
        else if (i == 1)
            cache[i] = (seq_t)2;          /* a(1) */
        else if (i == 2)
            cache[i] = (seq_t)4;          /* a(2) */
        else if (i == 3)
            cache[i] = (seq_t)7;          /* a(3) */
        else if (i == 4)
            cache[i] = (seq_t)15;         /* a(4) */
        else {
            /* recurrence: a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5) */
            seq_t v = cache[i-1];
            v = v + cache[i-2] * (seq_t)3;
            v = v - cache[i-3] * (seq_t)2;
            v = v - cache[i-4];
            v = v + cache[i-5];
            cache[i] = v;
        }
    }

    cache_len = needed;     /* update stored length */
    return cache[(size_t)n];
}
