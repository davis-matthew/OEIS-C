/*
 * A242636  Number of tilings of a 4 X n rectangle using tetrominoes of shapes
 *          L, Z, O.
 * https://oeis.org/A242636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Recurrence (from the rational generating function):
   a(n) = 6*a(n-2) + 13*a(n-3) + 3*a(n-4) - 18*a(n-5) - 13*a(n-6)
          - 3*a(n-7) + a(n-8) - 2*a(n-9) - 4*a(n-10) - 2*a(n-12)
   for n ≥ 12.
   Initial terms (n = 0 … 11):
   1, 0, 3, 12, 23, 94, 289, 842, 2771, 8510, 26411, 83122. */

seq_t a(idx_t n) {
    /* cache of computed terms: cache[0 … last] */
    static seq_t *cache = NULL;
    static size_t cache_cap   = 0;   /* allocated size of cache   */
    static size_t cache_last  = 0;   /* index of the last value stored (0‑based) */
    static bool  initialized  = false;

    /* first 12 terms */
    static const seq_t init[12] = {
        1, 0, 3, 12, 23, 94, 289, 842,
        2771, 8510, 26411, 83122
    };

    if (n < 12)                     /* trivial case */
        return init[(size_t)n];

    /* first call: allocate cache and fill it with initial values */
    if (!initialized) {
        cache_cap  = 12;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_last  = 11;          /* last valid index so far */
        initialized = true;
    }

    /* grow cache if it does not already hold a[n] */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)(cache_last + 1); i <= n; ++i) {
        seq_t val = 0;

        val += cache[i - 2] * 6;           /* +6  a(n-2) */
        val += cache[i - 3] * 13;          /* +13 a(n-3) */
        val += cache[i - 4] * 3;           /* +3  a(n-4) */
        val -= cache[i - 5] * 18;          /* -18 a(n-5) */
        val -= cache[i - 6] * 13;          /* -13 a(n-6) */
        val -= cache[i - 7] * 3;           /* -3  a(n-7) */
        val += cache[i - 8];               /* +1  a(n-8) */
        val -= cache[i - 9] * 2;           /* -2  a(n-9) */
        val -= cache[i -10] * 4;           /* -4  a(n-10) */
        /* a(n-11) has coefficient 0 – skipped */
        val -= cache[i -12] * 2;           /* -2  a(n-12) */

        cache[i] = val;
    }
    cache_last = n;

    return cache[n];
}
