/*
 * A269465  Number of length-n 0..6 arrays with no repeated value equal to the
 *          previous repeated value.
 * https://oeis.org/A269465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 18*a(n-2) - 72*a(n-3) with initial values:
 *            a(1)=7, a(2)=49, a(3)=336
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 204 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  cached  = 0;

    if (n == 0) return 0;              /* not used but safe */

    /* initialise cache with the first three terms */
    if (cached == 0) {
        cached = 3;
        cache  = mem_new(seq_t, cached);
        cache[0] = (seq_t)7;
        cache[1] = (seq_t)49;
        cache[2] = (seq_t)336;
    }

    /* if we already have the term, return it */
    if (n <= cached) return cache[n-1];

    /* extend cache up to n */
    cache = mem_resize(seq_t, cache, n);
    for (idx_t i = cached; i < n; ++i) {
        seq_t next = 11*cache[i-1] - 18*cache[i-2] - 72*cache[i-3];
        cache[i] = next;
    }
    cached = n;
    return cache[n-1];
}
