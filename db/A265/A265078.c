/*
 * A265078  Partial sums of A072154.
 * https://oeis.org/A265078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 233, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A265078: Partial sums of A072154
 * Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7)
 * Initial values:
 *   a(0) = 1, a(1) = 4, a(2) = 9, a(3) = 16, a(4) = 25,
 *   a(5) = 37, a(6) = 52
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial table for the first 7 terms */
    static const seq_t init[7] = {1, 4, 9, 16, 25, 37, 52};

    /* dynamic cache for computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;   /* number of elements currently allocated */
    static idx_t last_computed = (idx_t)-1; /* highest n that has been computed */

    /* return immediately for the known initial terms */
    if (n < 7)
        return init[n];

    /* allocate cache if not yet done */
    if (cache == NULL) {
        cache_sz = 7;
        cache    = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = init[i];
        last_computed = 6;                  /* highest precalculated index */
    }

    /* if the requested term has already been computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* grow the cache if necessary */
    if ((idx_t)(n + 1) > cache_sz) {
        idx_t new_sz = cache_sz ? cache_sz * 2 : (idx_t)(n + 1);
        while (new_sz <= n)
            new_sz *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();                /* reallocation failure */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* compute missing terms using the linear recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - a(i-2) + a(i-5) - 2*a(i-6) + a(i-7) */
        seq_t term = 2 * cache[i - 1];
        term -= cache[i - 2];
        term += cache[i - 5];
        term -= 2 * cache[i - 6];
        term += cache[i - 7];
        cache[i] = term;
    }

    last_computed = n;
    return cache[n];
}
