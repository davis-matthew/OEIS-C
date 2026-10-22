/*
 * A326118  a(n) is the largest number of squares of unit area connected only
 *          at corners and without holes that can be inscribed in an n X n
 *          square.
 * https://oeis.org/A326118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 0, a(1) = 1, a(2) = 2 * For n > 8: a(n) = 2*a(n-1) -
 *            a(n-2) + a(n-4) - 2*a(n-5) + a(n-6) * (This recurrence follows
 *            from the closed form * a(n) = (-29 + 12*n + 2*n^2 - 3*(-1)^n -
 *            12*sin(n*PI/2))/8 for n>2)
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoised computation of the sequence */
seq_t a(idx_t n)
{
    /* initialise a small cache of known values 0..8 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* helper local constants */
    const seq_t init_vals[9] = {
        (seq_t)0, (seq_t)1, (seq_t)2,
        (seq_t)5, (seq_t)6, (seq_t)9,
        (seq_t)14, (seq_t)21, (seq_t)24
    };

    /* if first call, allocate cache and store initial values */
    if (cache_size == 0) {
        /* choose an initial capacity that can hold at least the
           first 9 terms; grow it automatically later */
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* abort on allocation failure */
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
    }

    /* enlarge cache if needed */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;        /* double until large enough */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;             /* abort on realloc failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (n > 8) {
        for (idx_t i = 9; i <= n; ++i) {
            /* use the recurrence:
             *   a(i) = 2*a(i-1) - a(i-2) + a(i-4) - 2*a(i-5) + a(i-6)
             */
            seq_t v = cache[i - 1];
            v += v;                         /* 2*a(i-1) */
            v += cache[i - 4];               /* + a(i-4) */
            v += cache[i - 6];               /* + a(i-6) */
            v -= cache[i - 2];               /* - a(i-2) */
            seq_t t = cache[i - 5];
            t += t;                         /* 2*a(i-5) */
            v -= t;                         /* - 2*a(i-5) */
            cache[i] = v;
        }
    }

    return cache[n];
}
