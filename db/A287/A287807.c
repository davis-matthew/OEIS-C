/*
 * A287807  Number of senary sequences of length n such that no two
 *          consecutive terms have distance 2.
 * https://oeis.org/A287807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2), a(0)=1, a(1)=6, a(2)=28
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)28;

    /* Static cache: lazy‑initialized and grown on demand */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of elements allocated */
    static size_t last_computed = 0; /* highest index already computed */

    /* First time: allocate space for the first 3 values */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 3);
        if (!cache) return (seq_t)0;          /* allocation failure fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)28;
        cache_cap = 3;
        last_computed = 2;
    }

    /* Grow the cache if the requested n is beyond its current size */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;      /* allocation failure fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to index n */
    if ((size_t)n > last_computed) {
        const seq_t six = (seq_t)6;
        for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] * six - cache[i - 2] * six;
        }
        last_computed = (size_t)n;
    }

    return cache[(size_t)n];
}
