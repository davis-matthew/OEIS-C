/*
 * A373238  Relative of Hofstadter Q-sequence: a(n) = max(0, n+200) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A373238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) with a(n) =
 *            max(0, n+200) for n <= 0. Offset: 1 (a(1) is the first term)
 * Range:     u8 n <= 23, u16 n <= 220, u32 n <= 220, u64 n <= 220,
 *            u128 n <= 220, big any n
 * Validated: full, pass, 220 terms through n = 220 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t a_signed(int64_t n) {
    /* Base case: n <= 0 */
    if (n <= 0) {
        int64_t val = n + 200;
        return (val > 0) ? (seq_t)val : (seq_t)0;
    }

    /* Sequence is defined only up to n = 220 */
    if (n > 220)
        return (seq_t)0;

    /* Static cache for computed values 1..220 */
    static seq_t *cache = NULL;
    static const size_t cache_cap = 221;  /* indices 0..220 */
    if (cache == NULL) {
        cache = mem_new(seq_t, cache_cap);
        if (!cache)
            return (seq_t)0;  /* allocation failure, return 0 */
        /* initialise with sentinel value (all bits 1) */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = (seq_t)(-1);
    }

    /* Cached value? */
    seq_t cached = cache[(size_t)n];
    if (cached != (seq_t)(-1))
        return cached;

    /* Recurrence calls */
    seq_t a1 = a_signed((int64_t)n - (int64_t)a_signed((int64_t)n - 1));
    seq_t a2 = a_signed((int64_t)n - (int64_t)a_signed((int64_t)n - 2));
    seq_t a3 = a_signed((int64_t)n - (int64_t)a_signed((int64_t)n - 3));

    seq_t result = a1 + a2 + a3;
    cache[(size_t)n] = result;
    return result;
}

seq_t a(idx_t n) {
    /* Forward wrapper: n is unsigned but we pass through signed helper */
    return a_signed((int64_t)n);
}
