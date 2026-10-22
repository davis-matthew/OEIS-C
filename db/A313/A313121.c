/*
 * A313121  Coordination sequence Gal.3.25.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + a(n-5) - a(n-8) for n > 10, a(0)=1, a(1)=4,
 *            a(2)=9, a(3)=14, a(4)=19, a(5)=25, a(6)=28, a(7)=33, a(8)=40,
 *            a(9)=42, a(10)=48
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Initial values for 0 … 10 */
    static const seq_t init[11] =
    {
        1, 4, 9, 14, 19, 25, 28, 33, 40, 42, 48
    };

    /* Static cache for the computed terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Make sure the cache can hold the requested index n. */
    if ((size_t)n >= cap)
    {
        size_t newcap = cap ? cap : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();            /* memory allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Copy the known initial values into the cache. */
    for (size_t i = 0; i <= n && i <= 10; ++i)
        cache[i] = init[i];

    /* Compute the terms for n > 10 using the recurrence. */
    for (idx_t i = 11; i <= n; ++i)
        cache[i] = cache[i-3] + cache[i-5] - cache[i-8];

    return cache[n];
}
