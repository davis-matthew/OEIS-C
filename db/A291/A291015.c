/*
 * A291015  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S^3)^2.
 * https://oeis.org/A291015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3) for n >= 4; a(0) = 2, a(1) =
 *            7, a(2) = 23, a(3) = 75
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 983 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (!cache) {
        /* initial capacity of 4 (indices 0..3) */
        cache = mem_new(seq_t, 4);
        if (!cache) return 0;            /* allocation failed – abort */
        cache[0] = 2;
        cache[1] = 7;
        cache[2] = 23;
        cache[3] = 75;
        cap = 4;
    }

    if ((size_t)n < cap)               /* value already known */
        return cache[n];

    /* grow cache up to required index */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) return 0;                /* allocation failed – abort */
    cache = tmp;

    /* compute terms from cap up to n using the recurrence */
    for (size_t i = cap; i < new_cap; ++i) {
        /* a(i) = 5*a(i-1) - 6*a(i-2) + a(i-3) */
        cache[i] = cache[i-1] * 5;
        cache[i] += cache[i-3];
        cache[i] -= cache[i-2] * 6;
    }

    cap = new_cap;
    return cache[n];
}
