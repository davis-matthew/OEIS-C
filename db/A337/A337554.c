/*
 * A337554  a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * (5*k-4) * a(n-k).
 * https://oeis.org/A337554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} binomial(n,k) * (5*k-4) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;          /* Cache for computed a(n) values */
static idx_t  cache_len  = 0;        /* Number of values currently cached */
static idx_t  cache_cap  = 0;        /* Current capacity of the cache */

/* Initialize the cache with the first term a(0)=1 */
static void init_cache(void)
{
    if (!cache) {
        cache_cap = 4;                            /* Initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;                      /* a(0) = 1 */
        cache_len = 1;
    }
}

seq_t a(idx_t n)
{
    /* Ensure cache is initialized */
    if (!cache) init_cache();

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Ensure the array is large enough */
    if ((size_t)(n + 1) > cache_cap)
        ensure_capacity((size_t)(n + 1), &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Compute terms from cache_len up to n */
    for (idx_t m = cache_len; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;                 /* binomial(m,0) */
        for (idx_t k = 1; k <= m; ++k) {
            /* Update binomial coefficient binom(m,k) */
            binom = binom * (seq_t)(m - k + 1) / (seq_t)k;
            seq_t coef = (seq_t)5 * (seq_t)k - (seq_t)4;
            sum += binom * coef * cache[m - k];
        }
        cache[m] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
