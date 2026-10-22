/*
 * A305472  a(0) = 1, a(1) = 3, a(n) = 3*n*a(n-1) - 2*a(n-2).
 * https://oeis.org/A305472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 3, a(n) = 3*n*a(n-1) - 2*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute A305472 sequence value for index n */
seq_t a(idx_t n)
{
    /* Static cache holds all already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;     /* current allocated length        */
    static idx_t cached_max = 1;    /* largest index for which a(..) is known */

    /* Allocate or grow cache on first use or when n exceeds capacity */
    if (!cache) {
        cache_cap = (n > 1) ? n + 1 : 2;   /* at least room for a[0] & a[1] */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    } else if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;      /* double until large enough */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n if needed */
    if (n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            /* a(i) = 3 * i * a(i-1) - 2 * a(i-2) */
            seq_t term = (seq_t)3 * (((seq_t)i) * cache[i - 1]);
            term = term - (seq_t)2 * cache[i - 2];
            cache[i] = term;
        }
        cached_max = n;
    }

    return cache[n];
}
