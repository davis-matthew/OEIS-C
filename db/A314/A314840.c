/*
 * A314840  Coordination sequence Gal.5.66.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314840
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

/* Recurrence: a(n) = a(n-1) + a(n-5) - a(n-6) for n > 6
   Initial values: a(0)=1, a(1)=5, a(2)=9, a(3)=14, a(4)=18, a(5)=23, a(6)=28
   Sequence A314840 (Coordination sequence Gal.5.66.3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of previously computed terms      */
    static size_t cap  = 0;       /* current capacity of the array           */
    static idx_t max   = 0;       /* largest index that has been computed    */

    /* initial values for indices 0..6 */
    const seq_t init[7] = { 1, 5, 9, 14, 18, 23, 28 };

    /* first call: allocate cache and store initial values */
    if (!cache)
    {
        cap = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (int i = 0; i < 7; ++i) cache[i] = init[i];
        max = 6;
    }

    /* if n is already computed, return it directly */
    if (n <= max)
        return cache[(size_t)n];

    /* enlarge the cache if necessary */
    if ((size_t)(n + 1) > cap)
    {
        cap = n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* compute the missing terms using the recurrence */
    for (idx_t i = max + 1; i <= n; ++i)
        cache[(size_t)i] = cache[(size_t)(i - 1)] + cache[(size_t)(i - 5)] - cache[(size_t)(i - 6)];

    max = n;
    return cache[(size_t)n];
}
