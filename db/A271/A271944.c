/*
 * A271944  Expansion of 2*x*(1 + x)/(1 - x - 9*x^2 + x^3).
 * https://oeis.org/A271944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   OEIS A271944 Recurrence: a(n) = a(n-1) + 9*a(n-2) - a(n-3)
 *            Initial values: a(0)=0, a(1)=2, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization; grows only when a larger n is requested. */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Grow the cache if necessary. */
    if (n >= cap) {
        size_t newcap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;

        /* Ensure the initial values are set. */
        if (cap > 0) cache[0] = (seq_t)0;
        if (cap > 1) cache[1] = (seq_t)2;
        if (cap > 2) cache[2] = (seq_t)4;
    }

    /* Compute any missing terms using the recurrence. */
    if (n >= 3) {
        for (size_t i = 3; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + (seq_t)9 * cache[i-2] - cache[i-3];
        }
    }

    return cache[n];
}
