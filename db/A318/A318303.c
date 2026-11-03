/*
 * A318303  a(0) = 0, a(n) = n + a(n-1) if n is odd, a(n) = -3*a(n/2) if n is
 *          even.
 * https://oeis.org/A318303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(0) = 0; a(n) = n + a(n-1) if n is odd; a(n) = -3*a(n/2) if n
 *            is even
 * Range:     i8 n <= 20, i16 n <= 647, i32 n <= 32767, i64 n <= 32767,
 *            i128 n <= 32767, big any n
 * Validated: standard, pass, 83 terms through n = 8906 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Cache of values that have already been computed. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Initialise the cache for n = 0. */
    if (cache == NULL) {
        cache = mem_new(seq_t, 1);
        if (!cache) return 0;           /* allocation failed */
        cache[0] = 0;
        cache_cap = 1;
    }

    /* Extend the cache if a larger index is requested. */
    if (n >= cache_cap) {
        idx_t old_cap = cache_cap;
        idx_t new_cap = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;

        for (idx_t i = old_cap; i <= n; ++i) {
            if (i & 1) {                /* odd i */
                cache[i] = cache[i - 1] + (seq_t)i;
            } else {                    /* even i */
                cache[i] = - ((seq_t)3 * cache[i >> 1]);
            }
        }
    }

    return cache[n];
}
