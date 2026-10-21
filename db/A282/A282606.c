/*
 * A282606  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 489", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 10000*a(n-4) - 10000*a(n-6) for n > 13 Initial
 *            values: a(0) = 1 a(1) = 0 a(2) = 111 a(3) = 100 a(4) = 11011
 *            a(5) = 11110 a(6) = 1110011 a(7) = 1011010 a(8) = 110111001 a(9)
 *            = 111101000 a(10) = 11100111001 a(11) = 10110101000 a(12) =
 *            1101110011001 a(13) = 1111010011000
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS A282606
 * (Binary representation of the x-axis, from the left edge to the origin,
 *  of the n-th stage of growth of the two-dimensional cellular automaton
 *  defined by "Rule 489", based on the 5-celled von Neumann neighborhood).  */
seq_t a(idx_t n)
{
    /* Base values for n = 0 .. 13 */
    static const seq_t base_vals[] = {
        1ULL,                 /* a(0) */
        0ULL,                 /* a(1) */
        111ULL,               /* a(2) */
        100ULL,               /* a(3) */
        11011ULL,             /* a(4) */
        11110ULL,             /* a(5) */
        1110011ULL,           /* a(6) */
        1011010ULL,           /* a(7) */
        110111001ULL,         /* a(8) */
        111101000ULL,         /* a(9) */
        11100111001ULL,       /* a(10) */
        10110101000ULL,       /* a(11) */
        1101110011001ULL,     /* a(12) */
        1111010011000ULL      /* a(13) */
    };
    const size_t base_len = sizeof(base_vals) / sizeof(base_vals[0]);

    if (n < base_len) {
        return base_vals[(size_t)n];
    }

    /* Static cache for already-computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored (cache_len = current maximal n + 1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure capacity for n+1 elements */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = needed * 2;   /* grow more than needed to reduce reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize cache with base values if empty or newly allocated */
    if (cache_len == 0) {
        for (size_t i = 0; i < base_len; ++i) {
            cache[i] = base_vals[i];
        }
        cache_len = base_len;
    }

    /* Compute terms from cache_len up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        /* a(i) = a(i-2) + 10000*a(i-4) - 10000*a(i-6)  (i > 13) */
        seq_t term = cache[i-2] +
                     ((seq_t)10000) * cache[i-4] -
                     ((seq_t)10000) * cache[i-6];
        cache[i] = term;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
