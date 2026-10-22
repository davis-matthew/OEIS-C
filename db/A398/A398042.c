/*
 * A398042  G.f. A(x) satisfies A(x) = A(x^3) / (1-4*x), with A(0) = 1.
 * https://oeis.org/A398042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + (n%3==0 ? a(n/3) : 0), a(0) = 1 Sequence: OEIS
 *            A398042
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 
   seq_t and idx_t are defined by the build system.
   This function uses a simple iterative recurrence and memoises
   previously computed values in a static array.  The array grows
   geometrically to accommodate arbitrary indices. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated size of the array      */
    static idx_t  last   = 0;   /* highest index that has been computed */

    /* initialise cache on first use */
    if (cache == NULL) {
        size_t init_cap = 1;            /* we need at least a[0] */
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
#endif
        if (cache == NULL) exit(1);     /* out‑of‑memory guard */
        cache[0] = 1;                   /* a(0) = 1 */
        cap  = init_cap;
        last = 0;
    }

    /* if requested index already known, just return it */
    if (n <= last) return cache[n];

    /* ensure the array is large enough */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap == 0 ? 1 : cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;          /* double until we have enough */

        seq_t *new_arr;
#ifdef __cplusplus
        new_arr = new seq_t[new_cap];
#else
        new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        if (new_arr == NULL) exit(1);            /* out‑of‑memory guard */

        /* copy existing values */
        for (size_t i = 0; i < cap; ++i)
            new_arr[i] = cache[i];

        /* initialise the rest (necessary only for C) */
#ifndef __cplusplus
        for (size_t i = cap; i < new_cap; ++i)
            new_arr[i] = 0;
#endif

        /* free old storage */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_arr;
        cap   = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 4 * cache[i - 1];
        if (i % 3 == 0)
            val += cache[i / 3];
        cache[i] = val;
    }
    last = n;

    return cache[n];
}
