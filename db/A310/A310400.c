/*
 * A310400  Coordination sequence Gal.6.337.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A310400 (Coordination sequence Gal.6.337.1)
 *
 *  Conjectured recurrence (n > 10):
 *      a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) +
 *             a(n-9) - a(n-10)
 *  Initial values (n = 0 … 10):
 *      1, 4, 10, 14, 19, 24, 29, 34, 39, 44, 48
 *
 *  The implementation lazily memoises the sequence.  A static array
 *  is grown only when a larger index is requested.
 */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are defined by the build system.  No redeclaration is
 * required here. */

/* static cache and its capacity (size in terms of seq_t elements) */
static seq_t *cache = NULL;
static size_t cap   = 0;

/* initial values a(0) … a(10) */
static const seq_t init[11] = { 1, 4, 10, 14, 19, 24, 29, 34, 39, 44, 48 };

/* Main function required by the build system */
seq_t a(idx_t n)
{
    /* If the request is within the fixed initial block, return directly. */
    if (n < 11U) {
        return init[(size_t)n];
    }

    /* How many terms are needed?  Use size_t for internal indexing. */
    size_t needed = (size_t)n + 1U;
    if (needed < 11U) needed = 11U;  /* never contract below 11 */

    /* Grow the cache if necessary. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (cache == NULL) {
            /* Memory allocation failed – return 0 as a defensive value. */
            return 0;
        }
        /* Copy the initial 11 terms. */
        for (size_t i = 0; i < 11U; ++i) {
            cache[i] = init[i];
        }
        /* Compute up to 'needed - 1'. */
        for (size_t i = 11U; i < needed; ++i) {
            cache[i] = cache[i-1] + cache[i-3]
                     - cache[i-4] - cache[i-6]
                     + cache[i-7] + cache[i-9]
                     - cache[i-10];
        }
        cap = needed;
    } else if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) {
            /* Reallocation failed – keep the old cache and return 0. */
            return 0;
        }
        cache = tmp;
        /* Compute the newly required terms. */
        for (size_t i = cap; i < needed; ++i) {
            cache[i] = cache[i-1] + cache[i-3]
                     - cache[i-4] - cache[i-6]
                     + cache[i-7] + cache[i-9]
                     - cache[i-10];
        }
        cap = needed;
    }

    /* Return the requested term. */
    return cache[(size_t)n];
}
