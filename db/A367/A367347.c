/*
 * A367347  a(1) = 1; thereafter, a(n+1) = a(n) + x + 10*y, where x and y are
 *          respectively the first and last digits of a(n).
 * https://oeis.org/A367347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n+1) = a(n) + first_digit(a(n)) +
 *            10*last_digit(a(n))
 * Range:     u8 n <= 8, u16 n <= 1365, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 70 terms through n = 9181 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute a(n).  Any order of calls is supported via a static cache. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;            /* undefined for n < 1, guard */

    static seq_t *cache    = NULL;           /* a(1) is at cache[0] */
    static idx_t used      = 0;              /* how many terms are filled */
    static idx_t capacity  = 0;              /* allocated space */

    /* First call: initialise cache with a(1) */
    if (!cache) {
        capacity = 1;
        cache = mem_new(seq_t, capacity);
        if (!cache) abort();
        cache[0] = (seq_t)1;                 /* a(1) = 1 */
        used = 1;
    }

    /* Grow the buffer so that it can hold at least n elements. */
    while (capacity < n) {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        seq_t *tmp = mem_resize(seq_t, cache, capacity);
        if (!tmp) abort();
        cache = tmp;
    }

    /* Generate missing terms up to n. */
    for (idx_t i = used; i < n; ++i) {
        seq_t prev = cache[i - 1];

        /* last digit: prev % 10 */
        seq_t last = prev % 10u;

        /* first (most significant) digit: divide until < 10 */
        seq_t tmp = prev;
        while (tmp >= 10u) tmp /= 10u;
        seq_t first = tmp;

        /* next term */
        cache[i] = prev + first + (seq_t)10u * last;
    }

    used = n;                                 /* all terms up to n are ready */
    return cache[n - 1];
}
