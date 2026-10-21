/*
 * A283660  Number of n X 2 0..1 arrays with no element unequal to more than
 *          four of its king-move neighbors, with the exception of exactly two
 *          elements, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A283660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 54*a(n-2) + 124*a(n-3) - 204*a(n-4) +
 *            288*a(n-5) - 248*a(n-6) + 240*a(n-7) - 96*a(n-8) + 64*a(n-9) for
 *            n ≥ 10 Initial values: a(1)=0 a(2)=0 a(3)=0 a(4)=0 a(5)=4
 *            a(6)=40 a(7)=264 a(8)=1504 a(9)=7936
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mul_small.h"

/* The sequence function.  External linkage, no static helpers except internal. */
seq_t a(idx_t n)
{
    /* Lazily allocated memoisation cache */
    static seq_t *cache       = NULL;   /* cache[0] unused; cache[1] = a(1) etc. */
    static size_t cache_cap   = 0;       /* allocated capacity (in elements) */
    static idx_t  computed_max = 0;     /* highest n for which cache[n] is valid */

    if (n < 1) return (seq_t)0;          /* the sequence is defined for n≥1 only */

    /* Ensure the cache can hold element n (and the first 9 preset values). */
    size_t needed = (size_t)n + 1;        /* indices 0..n inclusive */
    if (needed < 10) needed = 10;        /* space for a(1) .. a(9) */

    if (needed > cache_cap) {
        size_t new_cap = needed;
        seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_cache) abort();         /* out‑of‑memory */

        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache      = new_cache;
        cache_cap  = new_cap;

        /* Initialise the first 9 terms the first time the cache is created. */
        if (computed_max == 0) {
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)0;
            cache[3] = (seq_t)0;
            cache[4] = (seq_t)0;
            cache[5] = (seq_t)4;
            cache[6] = (seq_t)40;
            cache[7] = (seq_t)264;
            cache[8] = (seq_t)1504;
            cache[9] = (seq_t)7936;
            computed_max = 9;
        }
    }

    /* If the requested term is already cached, return it. */
    if (n <= (idx_t)computed_max) return cache[(size_t)n];

    /* Compute terms from computed_max+1 up to n using the recurrence. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i >= 10) {
            seq_t val =  mul_small(cache[(size_t)(i-1)], 12)
                      - mul_small(cache[(size_t)(i-2)], 54)
                      + mul_small(cache[(size_t)(i-3)], 124)
                      - mul_small(cache[(size_t)(i-4)], 204)
                      + mul_small(cache[(size_t)(i-5)], 288)
                      - mul_small(cache[(size_t)(i-6)], 248)
                      + mul_small(cache[(size_t)(i-7)], 240)
                      - mul_small(cache[(size_t)(i-8)], 96)
                      + mul_small(cache[(size_t)(i-9)], 64);
            cache[(size_t)i] = val;
        }
        /* for i = 1..9 the values are already initialised */
    }

    computed_max = n;
    return cache[(size_t)n];
}
