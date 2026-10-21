/*
 * A291414  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 2 S + S^3.
 * https://oeis.org/A291414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-a(n-3)-3*a(n-4)-3*a(n-5)-a(n-6) for
 *            n>=7 Initial values: a(0)=2, a(1)=6, a(2)=15, a(3)=37, a(4)=89,
 *            a(5)=212, a(6)=500
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;        /* array of memoized terms   */
    static idx_t cache_cap = 0;        /* current capacity of cache */
    static const seq_t init[7] = {2, 6, 15, 37, 89, 212, 500};

    /* Allocate or grow the cache to guarantee room for n+1 entries   */
    if (n >= cache_cap) {
        /* Determine new capacity: at least 7, at least n+1, double size for amortised O(1) growth */
        idx_t new_cap = (cache_cap == 0) ? 7 : cache_cap * 2;
        while (new_cap <= n)
            new_cap *= 2;
        /* realloc in C++ requires an explicit cast */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* initialise newly allocated slots to zero */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
        /* initialise the first seven elements on first allocation only */
        if (cache_cap == new_cap) {
            for (idx_t i = 0; i < 7; ++i)
                cache[i] = init[i];
        }
    }

    /* Expand the cache up to index n using the recurrence */
    for (idx_t i = 7; i <= n; ++i) {
        if (cache[i] != 0)          /* already computed */
            continue;
        seq_t val = 0;
        val = 2 * cache[i - 1]
            + 2 * cache[i - 2]
            -   cache[i - 3]
            - 3 * cache[i - 4]
            - 3 * cache[i - 5]
            -   cache[i - 6];
        cache[i] = val;
    }

    return cache[n];
}
