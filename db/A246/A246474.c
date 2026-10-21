/*
 * A246474  Number of length n+3 0..3 arrays with no pair in any consecutive
 *          four terms totalling exactly 3.
 * https://oeis.org/A246474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 51, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + a(n-3); a(1)=60, a(2)=132, a(3)=292 */
/* a(n) is defined for n ≥ 1; behaviour for lower indices is undefined */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization: cache[i] holds a(i) for i ≥ 1 */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_last = 0;   /* highest index that has been computed */

    /* --- one‑time initialization --- */
    if (cache == NULL) {
        cache_cap = 4;                     /* room for indices 0..3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        /* initialize indices 0..3; index 0 is unused */
        cache[0] = 0;
        cache[1] = 60;
        cache[2] = 132;
        cache[3] = 292;

        /* set any remaining entries to 0 (none yet) */
        for (size_t i = 4; i < cache_cap; ++i) cache[i] = 0;

        cache_last = 3;
    }

    /* guard against indices below the offset */
    if (n < 1) return (seq_t)0;

    /* use a size_t variable to avoid signed arithmetic on unsigned n */
    size_t idx = (size_t)n;

    /* if already computed, return it immediately */
    if (idx <= cache_last) {
        return cache[idx];
    }

    /* enlarge cache if necessary */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (idx >= new_cap) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;       /* allocation failure */
        /* zero the newly allocated part */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to idx */
    for (size_t i = cache_last + 1; i <= idx; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + a(i-3) */
        cache[i] = (seq_t)2 * cache[i - 1] + cache[i - 3];
    }
    cache_last = idx;

    return cache[idx];
}
