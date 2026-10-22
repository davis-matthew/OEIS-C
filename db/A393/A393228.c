/*
 * A393228  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - x)^3) / (1 - x)^2.
 * https://oeis.org/A393228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 ; a(n) = Sum_{k=0..n-1} binomial(n+2*k , n-k-1) * a(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_cache, binomial, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_cap = 0;         /* allocated size of cache  */
static idx_t   computed_upto = 0;    /* highest index that has been computed (cache[0] is set to 1) */

seq_t a(idx_t n)
{
    /* handle the base case immediately */
    if (n == 0)
        return (seq_t)1;

    /* Allocate or grow the cache as needed */
    if (cache == NULL)
    {
        cache_cap = (size_t)(n + 1);
        cache = alloc_cache(cache_cap);          /* zero‑initialized by the allocator */
        cache[0] = (seq_t)1;                     /* initialise a(0) */
        computed_upto = 0;
    }
    else if ((size_t)n >= cache_cap)            /* grow cache */
    {
        size_t old_cap = cache_cap;
        cache_cap = (size_t)(n + 1);
        seq_t *new_cache = alloc_cache(cache_cap);
        for (size_t i = 0; i < old_cap; ++i)
            new_cache[i] = cache[i];
        free_cache(cache);
        cache = new_cache;
    }

    /* Compute any terms that have not yet been computed */
    for (idx_t m = computed_upto + 1; m <= n; ++m)
    {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k)
        {
            idx_t a_val = m + 2 * k;
            idx_t b_val = m - k - 1;
            seq_t coeff = binomial(a_val, b_val);
            sum += coeff * cache[k];
        }
        cache[m] = sum;
        computed_upto = m;
    }

    return cache[n];
}
