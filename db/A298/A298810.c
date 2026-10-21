/*
 * A298810  Growth series for group with presentation < S, T : S^2 = T^3 =
 *          (S*T)^8 = 1 >.
 * https://oeis.org/A298810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 3*a(n-6) + a(n-8) - a(n-12) for n>12, with the
 *            additional constant -2 for n = 14, derived from the generating
 *            function numerator
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 77, u64 n <= 158,
 *            u128 n <= 322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

static const seq_t init_terms[13] = {
    1, 3, 4, 6, 8, 12, 16, 24, 30, 39, 50, 69, 88
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t   cap   = 0;    /* allocated capacity */
    static size_t   size  = 0;    /* number of values stored */

    /* Immediate return for the known initial segment. */
    if (n <= 12) {
        return init_terms[(size_t)n];
    }

    /* First call: allocate cache and copy the initial values. */
    if (size == 0) {
        cap   = 64;                      /* arbitrary starting size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */
        for (size_t i = 0; i <= 12; ++i) {
            cache[i] = init_terms[i];
        }
        size = 13;
    }

    /* Grow the cache if the requested index is beyond the current size. */
    if ((size_t)n >= size) {
        /* Ensure we have enough capacity. */
        if ((size_t)n + 1 > cap) {
            size_t new_cap = cap;
            while ((size_t)n + 1 > new_cap) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }
        /* Compute missing terms up to n using the recurrence. */
        for (size_t i = size; i <= (size_t)n; ++i) {
            seq_t v = cache[i - 4]
                    + 3 * cache[i - 6]
                    +       cache[i - 8]
                    -       cache[i - 12];
            if (i == 14) {                /* adjustment from numerator */
                v -= (seq_t)2;
            }
            cache[i] = v;
        }
        size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
