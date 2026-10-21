/*
 * A285184  a(n) = 2*a(n-1) + a(n-3) with initial terms 1,3,5.
 * https://oeis.org/A285184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-3) Initial values: a(0) = 1, a(1) = 3,
 *            a(2) = 5
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache to store computed terms */
    static seq_t *cache    = NULL;
    static idx_t cache_cap = 0; /* allocated capacity */
    static idx_t cache_sz  = 0; /* number of computed elements */

    /* Expand the cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= n) new_cap = n + 1; /* at least n+1 slots */

        /* realloc must be cast in C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to index n */
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = 1;
        else if (i == 1)     val = 3;
        else if (i == 2)     val = 5;
        else                 val = 2 * cache[i - 1] + cache[i - 3];
        cache[i] = val;
    }
    cache_sz = n + 1;

    return cache[n];
}
