/*
 * A375273  Expansion of 1/(1 - 2*x - 3*x^2 - 4*x^3 + 4*x^4).
 * https://oeis.org/A375273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expand 1/(1 - 2*x - 3*x^2 - 4*x^3 + 4*x^4) using the recurrence
 * a(n) = 2*a(n-1) + 3*a(n-2) + 4*a(n-3) - 4*a(n-4)
 * with initial values a(0)=1, a(1)=2, a(2)=7, a(3)=24.  The function
 * memoises previously computed terms so that arbitrary n can be
 * queried in any order. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)24;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure capacity for indices up to n */
    size_t required = (size_t)n + 1;
    if (cap < required) {
        seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure fallback */

        /* Copy existing data */
        if (cache && cap > 0)
            memcpy(new_cache, cache, cap * sizeof(seq_t));

        /* If we are allocating for the first time, initialise first four values */
        if (cap == 0) {
            new_cache[0] = (seq_t)1;
            new_cache[1] = (seq_t)2;
            new_cache[2] = (seq_t)7;
            new_cache[3] = (seq_t)24;
        }

        /* Free old storage if any */
        free(cache);
        cache = new_cache;
        cap = required;
    }

    /* Compute any missing terms up to n */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        cache[i] =
            cache[i-1] * (seq_t)2
          + cache[i-2] * (seq_t)3
          + cache[i-3] * (seq_t)4
          - cache[i-4] * (seq_t)4;
    }

    return cache[(size_t)n];
}
