/*
 * A242510  Number of n-length words on {1,2,3} such that the maximal blocks
 *          (runs) of 1's have odd length, the maximal blocks of 2's have even
 *          length and the maximal blocks of 3's have odd length.
 * https://oeis.org/A242510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - a(n-4) with initial values
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=8
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of already computed terms.  All accesses are protected by
   a single static cache that grows on demand. */
static seq_t *cache = NULL;          /* pointer to first element */
static idx_t cache_cap = 0;          /* number of elements currently stored */

/* Initial values for the first four indices. */
static const seq_t init[4] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)8 };

/* The sequence function. */
seq_t a(idx_t n)
{
    /* For indices smaller than 4 we can answer directly. */
    if (n < 4)
        return init[(int)n];

    /* If the cache is not large enough, grow it to n+1 elements. */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);

        /* Allocation: use realloc in a way that is valid in both C and C++. */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                 /* allocation failure */

        cache = new_cache;
        cache_cap = new_cap;

        /* If the cache was empty, initialize the first four terms. */
        if (cache_cap == (idx_t)new_cap && cache_cap >= 4) {
            /* Newly allocated memory may be uninitialized; fill it. */
            for (idx_t i = 0; i < 4; ++i)
                cache[i] = init[i];
        } else if (cache_cap == (idx_t)new_cap && cache_cap < 4) {
            /* This can happen when realloc was called for the very first
               time and the capacity is exactly 4.  In that case we
               still need to copy the initial values. */
            for (idx_t i = 0; i < cache_cap; ++i)
                cache[i] = init[i];
        }

        /* If cache already had values beyond the initial block,
           the values from cache_cap_old to 3 (if any) were already
           computed earlier.  So compute from the maximum of
           (previous capacity, 4) up to n. */
        idx_t start = 4;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = cache[i-1] + (cache[i-2] * (seq_t)2) + cache[i-3] - cache[i-4];
        }
    }

    return cache[n];
}
