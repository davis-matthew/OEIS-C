/*
 * A323716  a(n) = Product_{k=0..n} (3^k + 1).
 * https://oeis.org/A323716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (3^k + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow3.h"   /* provides seq_t pow3(idx_t exp) */

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t    maxn  = (idx_t)-1;   /* not yet initialized */

    /* Initialisation on first call */
    if (maxn == (idx_t)-1) {
        maxn = 0;
        cap   = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 2;                     /* 3^0 + 1 = 2 */
    }

    /* If the requested n has already been computed, return it */
    if (n <= maxn) {
        return cache[n];
    }

    /* Grow the cache to accommodate n */
    size_t new_cap = cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap   = new_cap;

    /* Compute terms from maxn+1 up to n */
    for (idx_t i = maxn + 1; i <= n; ++i) {
        seq_t term = pow3(i) + 1;          /* 3^i + 1 */
        cache[i] = cache[i-1] * term;      /* cumulative product */
    }

    maxn = n;
    return cache[n];
}
