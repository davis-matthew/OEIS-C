/*
 * A284244  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 841", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 100*a(n-2) + a(n-4) - 100*a(n-6)  for n>16
 * Initial values (offset 0):
 * a(0) = 1, a(1) = 0, a(2) = 110, a(3) = 1110, a(4) = 11110,
 * a(5) = 101110, a(6) = 101110, a(7) = 10111110, a(8) = 11111110,
 * a(9) = 1110101110, a(10) = 11010101110, a(11) = 101010111110,
 * a(12) = 101011111110, a(13) = 10101110101110, a(14) = 10101010101110,
 * a(15) = 1010101010111110, a(16) = 1010101011111110
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values known from the OEIS entry */
    static const seq_t base[17] = {
        1ULL,                          /* a(0) */
        0ULL,                          /* a(1) */
        110ULL,                        /* a(2) */
        1110ULL,                       /* a(3) */
        11110ULL,                      /* a(4) */
        101110ULL,                     /* a(5) */
        101110ULL,                     /* a(6) */
        10111110ULL,                   /* a(7) */
        11111110ULL,                   /* a(8) */
        1110101110ULL,                 /* a(9) */
        11010101110ULL,                /* a(10) */
        101010111110ULL,               /* a(11) */
        101011111110ULL,               /* a(12) */
        10101110101110ULL,             /* a(13) */
        10101010101110ULL,             /* a(14) */
        1010101010111110ULL,           /* a(15) */
        1010101011111110ULL            /* a(16) */
    };

    /* Fast-path for the first 17 (offset 0) terms */
    if (n < 17) {
        return base[(size_t)n];
    }

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;  /* number of entries actually populated */
    static size_t cache_cap  = 0;  /* allocated capacity */

    /* Allocate and initialise cache on first entry */
    if (cache == NULL) {
        /* start with capacity for 17 terms */
        cache_cap = 17;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) {
            /* allocation failure – return zero as a fallback */
            return (seq_t)0;
        }
        /* copy base values */
        for (size_t i = 0; i < 17; ++i) {
            cache[i] = base[i];
        }
        cache_len = 17;
    }

    /* Grow cache capacity if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) {
            /* allocation failure – return zero as a fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    size_t start = (cache_len > 17) ? cache_len : 17;
    for (size_t i = start; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 100*a(i-2) + a(i-4) - 100*a(i-6) */
        seq_t temp = (seq_t)100 * cache[i - 2] + cache[i - 4];
        cache[i]   = temp - (seq_t)100 * cache[i - 6];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
