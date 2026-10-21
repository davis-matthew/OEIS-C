/*
 * A291218  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S^5.
 * https://oeis.org/A291218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) - 10*a(n-4) + a(n-5) + 10*a(n-6) - 5*a(n-8) +
 *            a(n-10) for n≥11 Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=1, a(5)=0, a(6)=5, a(7)=0, a(8)=15, a(9)=1, a(10)=35
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 50, u64 n <= 96, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

extern idx_t _floor_isqrt_u64(idx_t);   /* dummy extern to avoid empty file warning */

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* pointer to cached terms */
static size_t cache_cap = 0;         /* allocated capacity (number of slots) */
static size_t computed = 10;         /* index of largest computed term (≤ cache_cap-1) */

/* Initial terms a(0)..a(10) */
static const seq_t init_vals[11] = {
    0, 0, 0, 0, 1, 0, 5, 0, 15, 1, 35
};

/* Grow the cache to at least new_cap slots */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)                /* allocation failed – give up */
        return;
    cache = new_cache;
    /* initialise new elements (they will be overwritten when computed) */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

/* Ensure that terms up to n are present in the cache */
static void ensure_terms(idx_t n)
{
    if (cache == NULL) {
        /* first allocation – at least space for the initial 11 terms */
        cache_cap = 64;                 /* arbitrary small start */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)                     /* allocation failed – give up */
            return;
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init_vals[i];
        for (size_t i = 11; i < cache_cap; ++i)
            cache[i] = 0;
        computed = 10;
    }

    /* Resize if necessary */
    if ((size_t)n + 1 > cache_cap)
        grow_cache((size_t)n + 1);

    /* Compute missing terms */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        /* recurrence: a(n) = 5*a(n-2) - 10*a(n-4) + a(n-5) + 10*a(n-6)
                        - 5*a(n-8) + a(n-10) */
        seq_t val = 5 * cache[i - 2];
        val -= 10 * cache[i - 4];
        val +=   cache[i - 5];
        val += 10 * cache[i - 6];
        val -= 5 * cache[i - 8];
        val +=   cache[i -10];
        cache[i] = val;
    }
    computed = (size_t)n;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n <= 10)
        return init_vals[n];
    ensure_terms(n);
    /* If allocation failed, cache[n] will be 0 – acceptable as a fallback */
    return cache[(size_t)n];
}
