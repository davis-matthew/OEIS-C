/*
 * A393313  G.f. A(x) satisfies: A(x) = (1 + x + x^2)^2 * A(x^3) / (1 - x).
 * https://oeis.org/A393313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393313:
   a(0)=1, a(1)=3, a(2)=6, a(3)=11.
   For n>3:
   a(n) = a(n-3) + a(floor(n/3))
          + 2*a(floor((n-1)/3))
          + 3*a(floor((n-2)/3))
          + 2*a(floor((n-3)/3))
          + a(floor((n-4)/3)) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* return small base cases immediately (no caching needed) */
    if (n <= 3) {
        static const seq_t init[4] = { (seq_t)1,
                                       (seq_t)3,
                                       (seq_t)6,
                                       (seq_t)11 };
        return init[(size_t)n];
    }

    /* cache of previously computed values */
    static seq_t *cache      = NULL;   /* array of a(i) */
    static size_t cache_cap  = 0;      /* allocated size of cache */
    static size_t cache_size = 0;      /* number of values stored */

    /* initialise the cache if this is the first call */
    if (cache == NULL) {
        /* allocate at least 4 slots, but grow to cover n+1 if larger */
        cache_cap = 4;
        if ((size_t)(n + 1) > cache_cap)
            cache_cap = (size_t)(n + 1);

        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();

        /* set the initial values for indices 0..3 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)11;
        cache_size = 4;          /* we have computed up to index 3 */
    }
    /* grow the cache if necessary */
    else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values from cache_size up to n */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t v = cache[i - 3];
        v += cache[i / 3];
        v += cache[(i - 1) / 3] * 2;
        v += cache[(i - 2) / 3] * 3;
        v += cache[(i - 3) / 3] * 2;
        v += cache[(i - 4) / 3];
        cache[i] = v;
    }
    cache_size = (size_t)(n + 1);

    return cache[n];
}
