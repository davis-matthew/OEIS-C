/*
 * A290892  p-INVERT of the positive integers, where p(S) = 1 - S^4.
 * https://oeis.org/A290892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 970 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 69*a(n-4)
                     + 56*a(n-5) - 28*a(n-6) +   8*a(n-7) - a(n-8),
   a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 8, a(5) = 36, a(6) = 120, a(7) = 331.  */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Base values for n = 0..7 */
    static const seq_t base[8] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1,
        (seq_t)8, (seq_t)36, (seq_t)120, (seq_t)331
    };

    /* Static cache for computed values (zero-initialised at program start) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If n is within the base range, return directly */
    if (n < 8)
        return base[n];

    /* Initialise cache with base values if not yet done */
    if (cache_size == 0) {
        cache_size = 8;
        cache = mem_new(seq_t, cache_size);
        if (!cache)
            return (seq_t)0;  /* Allocation failure (unlikely) */
        mem_copy(cache, base, cache_size * sizeof(seq_t));
    }

    /* Extend cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp)
            return (seq_t)0;  /* Allocation failure (unlikely) */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] =
                ( (seq_t)8  * cache[i-1]
                - (seq_t)28 * cache[i-2]
                + (seq_t)56 * cache[i-3]
                - (seq_t)69 * cache[i-4]
                + (seq_t)56 * cache[i-5]
                - (seq_t)28 * cache[i-6]
                + (seq_t)8  * cache[i-7]
                -           cache[i-8] );
        }
        cache_size = new_size;
    }

    return cache[n];
}
