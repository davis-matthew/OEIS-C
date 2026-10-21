/*
 * A291731  a(n) = (1/2)*A291730(n).
 * https://oeis.org/A291731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 9, a(3) = 28, a(4) = 84, a(5) = 255
 *            a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) + 4*a(n-4) + 2*a(n-6) for
 *            n >= 6 (the recurrence contains no a(n‑5) term)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;   /* stores a[0] .. a[max_cached] */
    static size_t  cap   = 0;     /* current allocation size */
    static idx_t   max_cached = (idx_t)-1;  /* last index that is valid in cache */

    /* Base terms, size 6 */
    static const seq_t base[6] = {1, 3, 9, 28, 84, 255};

    /* If n is among the base terms, return it immediately */
    if (n < 6) {
        return base[n];
    }

    /* Initialise the cache on the first call */
    if (!cache) {
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* If allocation fails, return an arbitrary value (zero) as a safe fallback */
        if (!cache) {
            return 0;
        }
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = base[i];
        max_cached = 5;
    }

    /* If the requested index is already in cache, return it */
    if (n <= max_cached) {
        return cache[n];
    }

    /* Ensure capacity to hold up to index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap > 0 ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            return 0; /* allocation failure fallback */
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute successive terms using the 6‑term recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 2*a(i-2) + 2*a(i-3) + 4*a(i-4) + 2*a(i-6) */
        seq_t val = (2 * cache[i - 1]) +
                    (2 * cache[i - 2]) +
                    (2 * cache[i - 3]) +
                    (4 * cache[i - 4]) +
                    (2 * cache[i - 6]);
        cache[i] = val;
    }

    max_cached = n;
    return cache[n];
}
