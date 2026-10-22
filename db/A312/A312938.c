/*
 * A312938  Coordination sequence Gal.5.127.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + a(n-9) - a(n-12) for n > 12 Initial values:
 *            a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=18, a(5)=23, a(6)=26,
 *            a(7)=32, a(8)=37, a(9)=39, a(10)=45, a(11)=50, a(12)=52 The
 *            sequence is defined for all n ≥ 0. The implementation memoises
 *            computed terms so that calls in any order remain effic...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* the function definition required by the build */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of elements in cache  */
    /* initial values up to a(12) */
    static const seq_t init[13] = {1, 4, 9, 13, 18, 23, 26, 32, 37, 39, 45, 50, 52};

    /* If n is in the initial range return the table value directly */
    if (n < 13)
        return init[(size_t)n];

    /* Make sure the cache can hold the value at index n */
    if (cap == 0)
    {
        /* allocate at least 13 entries for the initial part */
        cap = (n + 1 > 13) ? (n + 1) : 13;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache)
            return 0;  /* memory failure – unlikely in test harness */
        /* initialise the first 13 entries */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        /* initialise the rest up to cap-1 with 0; they will be computed later */
        for (size_t i = 13; i < cap; ++i)
            cache[i] = 0;
    }
    else if ((size_t)n + 1 > cap)
    {
        /* grow the cache to accommodate n */
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;  /* memory failure */
        cache = tmp;
        /* initialise the new slots to 0 */
        for (size_t i = cap; i < newcap; ++i)
            cache[i] = 0;
        cap = newcap;
    }

    /* If the value is already computed, return it */
    if (cache[(size_t)n] != 0 || (size_t)n < 13)
        return cache[(size_t)n];

    /* Compute missing values up to n in increasing order */
    for (size_t i = 13; i <= (size_t)n; ++i)
    {
        if (cache[i] != 0) continue;  /* already computed */
        /* recurrence: a(i) = a(i-3) + a(i-9) - a(i-12) */
        seq_t term = cache[i - 3] + cache[i - 9] - cache[i - 12];
        cache[i] = term;
    }

    return cache[(size_t)n];
}
