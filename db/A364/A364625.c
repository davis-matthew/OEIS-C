/*
 * A364625  G.f. satisfies A(x) = 1/(1-x)^3 + x^2*A(x)^2.
 * https://oeis.org/A364625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A364625:  a(n) = binomial(n+2,2) + Σ_{k=0}^{n-2} a(k) * a(n-2-k),   a(0)=1, a(1)=3
   (derived from the functional equation A(x) = 1/(1-x)^3 + x^2*A(x)^2) */
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;      /* stored values a(0..len-1)          */
static idx_t  cache_len = 0;     /* number of elements stored          */
static size_t cache_cap = 0;     /* allocated capacity (in elements)   */

static void grow_cache(idx_t needed)
{
    if (needed < cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap <= (size_t)needed)
        newcap *= 2;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = newptr;
    cache_cap = newcap;
}

/* Entry point */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache can hold indices up to n. */
    grow_cache(n);

    /* Initialize base cases if they are not already present. */
    if (cache_len == 0) {
        cache[0] = binomial(2, 2);   /* a(0) = 1 */
        cache[1] = binomial(3, 2);   /* a(1) = 3 */
        cache_len = 2;
    }

    /* Compute values up to n inclusively. */
    for (idx_t m = cache_len; m <= n; ++m) {
        seq_t res;
        if (m < 2) {
            res = binomial(m + 2, 2);   /* 1 for m=0, 3 for m=1 */
        } else {
            /* Base term binomial(m+2,2) */
            res = binomial(m + 2, 2);

            /* Convolution sum a(k) * a(m-2-k) for k=0..m-2 */
            idx_t limit = m - 2;
            for (idx_t k = 0; k <= limit; ++k) {
                res += cache[k] * cache[limit - k];
            }
        }
        cache[m] = res;
    }

    cache_len = n + 1;
    return cache[n];
}
