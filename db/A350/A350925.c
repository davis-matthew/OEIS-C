/*
 * A350925  a(0) = 1, a(1) = 9, and a(n) = 16*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350925: a(0)=1, a(1)=9, a(n)=16*a(n-1)-a(n-2)-4 (n≥2) */
/* Computes each term with memoisation; works for uint64_t or arbitrary‑precision seq_t */

#include <stdlib.h>
#include <stdint.h>

static seq_t *alloc_array(idx_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t *)malloc(sizeof(seq_t) * sz);
#endif
}

static void free_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Return cached value if available */
    if (n < cache_size)
        return cache[n];

    /* Initialise cache if not yet allocated */
    if (cache == NULL) {
        cache_cap = 8;            /* small initial capacity */
        cache = alloc_array(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_size = 2;
    }

    /* Grow cache when necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;          /* double until large enough */

        /* Allocate new array, copy existing values, and free old array */
        seq_t *new_cache = alloc_array(new_cap);
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free_array(cache);

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute recurrence up to requested index */
    while (cache_size <= n) {
        idx_t i = cache_size;
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)16 * prev - prev2 - (seq_t)4;
        ++cache_size;
    }

    return cache[n];
}
