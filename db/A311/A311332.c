/*
 * A311332  Coordination sequence Gal.6.120.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) - a(n-6) +
 *            3*a(n-7) - 4*a(n-8) + 3*a(n-9) - a(n-10) (n > 10) with initial
 *            values a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=16, a(5)=21,
 *            a(6)=27, a(7)=33, a(8)=38, a(9)=42, a(10)=46
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    /* first 11 terms */
    static const seq_t init[11] = {1,4,8,12,16,21,27,33,38,42,46};

    /* ensure we can store indices up to n */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t newcap = needed > 11 ? needed : 11;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        assert(tmp != NULL);
        cache = tmp;
        /* initialize the first 11 entries */
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cap = newcap;
    }

    /* for n < 11 we already have the value */
    if ((size_t)n < 11)
        return cache[(size_t)n];

    /* compute up to n (might recompute earlier terms when n decreases) */
    for (size_t i = 11; i <= (size_t)n; ++i) {
        /* avoid intermediate underflow by grouping positive terms */
        seq_t pos = 3 * cache[i-1] + 3 * cache[i-3] + 3 * cache[i-7] + 3 * cache[i-9];
        seq_t neg = 4 * cache[i-2] + cache[i-4] + cache[i-6] + 4 * cache[i-8] + cache[i-10];
        cache[i] = pos - neg;
    }
    return cache[(size_t)n];
}
