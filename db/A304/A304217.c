/*
 * A304217  Number of nX4 0..1 arrays with every element unequal to 0, 1, 3 or
 *          8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 72, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for OEIS A304217:
   a(n) = a(n-1) + 2*a(n-3) + a(n-4) + a(n-5) - 2*a(n-7)   (n > 9)
   Initial values:
   a(1) = 5   a(2) = 17   a(3) = 17   a(4) = 32   a(5) = 62
   a(6) = 108 a(7) = 190  a(8) = 348  a(9) = 636
   All other terms are obtained by the recurrence. */
#include <stdlib.h>

static const seq_t init_vals[10] = {
    0,          /* dummy for index 0 */
    5,          /* a(1) */
    17,         /* a(2) */
    17,         /* a(3) */
    32,         /* a(4) */
    62,         /* a(5) */
    108,        /* a(6) */
    190,        /* a(7) */
    348,        /* a(8) */
    636         /* a(9) */
};

seq_t a(idx_t n)
{
    /* static cache: index 0 unused, indices 1..k contain computed terms */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;          /* number of allocated slots  (0..cache_sz-1) */

    /* Guard against zero or negative indices (should not occur for OEIS offset 1). */
    if (n == 0) return 0;

    /* Ensure cache exists and is initialized up to index 9. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 10);
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        for (size_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
        cache_sz = 10;
    }

    /* If requested index is already in the cache, return it. */
    if ((size_t)n < cache_sz) return cache[(size_t)n];

    /* Grow the cache to accommodate index n. */
    size_t new_sz = (size_t)n + 1;  /* indices 0..n inclusive */
    if (new_sz > cache_sz) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_sz);
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        /* Compute missing terms using the recurrence. */
        for (size_t i = cache_sz; i <= (size_t)n; ++i) {
            seq_t val = cache[i-1];                    /* a(n-1) */
            val += cache[i-3] * (seq_t)2;              /* 2*a(n-3) */
            val += cache[i-4];                         /* a(n-4) */
            val += cache[i-5];                         /* a(n-5) */
            val -= cache[i-7] * (seq_t)2;              /* 2*a(n-7) */
            cache[i] = val;
        }
        cache_sz = new_sz;
    }

    return cache[(size_t)n];
}
