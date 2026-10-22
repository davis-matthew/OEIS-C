/*
 * A301720  Coordination sequence for node of type V1 in "krb" 2-D tiling (or
 *          net).
 * https://oeis.org/A301720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) for n >= 7 a(0) = 1, a(1) = 6, a(2) =
 *            9, a(3) = 18, a(4) = 21, a(5) = 24, a(6) = 36 The sequence
 *            satisfies the linear recurrence derived from the generating
 *            function (x^6+6x^5+9x^4+16x^3+9x^2+6x+1)/(1-2x^3+x^6)
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of computed terms   */
static idx_t  cache_cap = 0;         /* number of elements cache can contain   */
static idx_t  max_computed = 0;      /* highest n for which a(n) has been computed */

/* Initialise cache with the first 7 terms when needed. */
static void init_cache(idx_t needed) {
    if (cache == NULL) {
        /* Allocate enough space for at least needed+1 elements. */
        cache_cap = needed + 1;
        if (cache_cap < 7) cache_cap = 7;      /* at least 7 for the base terms */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return;             /* allocation failure – unlikely for test data */

        /* Base terms of the sequence */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)24;
        cache[6] = (seq_t)36;
        max_computed = 6;
    } else if (needed + 1 > cache_cap) {
        /* Grow the cache if the new needed index exceeds current capacity. */
        cache_cap = needed + 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (new_ptr != NULL) cache = new_ptr;  /* keep old data */
    }
}

/* Compute a(n) using the recurrence with memoisation. */
seq_t a(idx_t n) {
    /* For small n, return the initial terms directly. */
    if (n <= 6) {
        static const seq_t init[7] = {
            (seq_t)1, (seq_t)6, (seq_t)9,
            (seq_t)18, (seq_t)21, (seq_t)24, (seq_t)36
        };
        return init[(size_t)n];
    }

    init_cache(n);          /* ensure we have space and initial values */

    if (n > max_computed) {
        /* Compute the sequence up to n using the recurrence. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 3] - cache[i - 6];
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
