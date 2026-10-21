/*
 * A281507  Trajectory of 1186061987030929990 under the "Reverse and Add!"
 *          operation.
 * https://oeis.org/A281507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1186061987030929990 Recurrence: a(n+1) = a(n) + rev(a(n))
 *            (reverse decimal digits)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 56, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 // malloc, realloc, exit
#include "../utils/reverse_decimal.h"  // reverse_decimal(seq_t)

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache  = NULL;
    static size_t cap     = 0;   /* allocated capacity */
    static size_t len     = 0;   /* number of values computed (next free index) */

    /* initialise base case on first call */
    if (len == 0) {
        if (cap == 0) {
            cap   = 1;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
        }
        cache[0] = (seq_t)1186061987030929990ULL;
        len      = 1;
    }

    /* extend cache if necessary */
    if ((size_t)n >= len) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t new_cap = cap ? (cap << 1) : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cap   = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            seq_t rev = reverse_decimal(cache[i - 1]); /* reverse decimal digits */
            cache[i] = cache[i - 1] + rev;             /* next term */
        }
        len = needed;
    }

    return cache[n];
}
