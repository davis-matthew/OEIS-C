/*
 * A250358  Number of length n arrays x(i), i=1..n with x(i) in i..i+5 and no
 *          value appearing more than 3 times.
 * https://oeis.org/A250358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -4*a(n-4) -22*a(n-5) -130*a(n-6) +7*a(n-8)
 *            +50*a(n-9) +66*a(n-10) +a(n-12) -6*a(n-13) Initial terms (offset
 *            1): a(1) = 6 a(2) = 36 a(3) = 216 a(4) = 1293 a(5) = 7712 a(6) =
 *            45866 a(7) = 272760 a(8) = 1621963 a(9) = 9644496 a(10) =
 *            57346376 a(11) = 340979312 a(12) = 2027446948 a(13...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Basic constant table */
    static const seq_t init[14] = {
        0ULL,          /* index 0 unused */
        6ULL,          /* a(1) */
        36ULL,         /* a(2) */
        216ULL,        /* a(3) */
        1293ULL,       /* a(4) */
        7712ULL,       /* a(5) */
        45866ULL,      /* a(6) */
        272760ULL,     /* a(7) */
        1621963ULL,    /* a(8) */
        9644496ULL,    /* a(9) */
        57346376ULL,   /* a(10) */
        340979312ULL,  /* a(11) */
        2027446948ULL, /* a(12) */
        12055094608ULL /* a(13) */
    };

    /* Return small values directly */
    if (n <= 13) {
        return init[(size_t)n];   /* cast for array indexing */
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static idx_t computed_up_to = 0; /* highest n computed */

    idx_t required = n;

    /* Allocate or extend the cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)(required + 1);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;  /* failed allocation, return 0 as a placeholder */
        for (int i = 1; i <= 13; ++i) {
            cache[i] = init[i];
        }
        computed_up_to = 13;
    } else if (cache_cap <= (size_t)required) {
        cache = (seq_t *)realloc(cache, (size_t)(required + 1) * sizeof(seq_t));
        if (!cache) return 0;  /* failed allocation */
        for (size_t i = cache_cap; i <= (size_t)required; ++i) {
            cache[i] = 0;
        }
        cache_cap = (size_t)(required + 1);
    }

    /* Compute up to the required index if not yet done */
    if (computed_up_to < required) {
        for (idx_t k = computed_up_to + 1; k <= required; ++k) {
            cache[k] =
                6ULL * cache[k-1]
              - 4ULL * cache[k-4]
              - 22ULL * cache[k-5]
              - 130ULL * cache[k-6]
              + 7ULL * cache[k-8]
              + 50ULL * cache[k-9]
              + 66ULL * cache[k-10]
              + 1ULL * cache[k-12]
              - 6ULL * cache[k-13];
        }
        computed_up_to = required;
    }

    return cache[n];
}
