/*
 * A400313  G.f. A(x) satisfies A(x) = A(x^3/(1 - x)^2) / (1 - x), with A(0) =
 *          1.
 * https://oeis.org/A400313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-k, 2*k) * a(k), a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 35, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recursive definition directly; values are cached
   to allow out-of-order calls. */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache of computed values and its capacity */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    /* number of values that have actually been computed (cache[0..len-1]) */
    static size_t cache_len = 0;

    /* Make sure the allocated array is large enough */
    if (n >= cache_cap) {
        /* ensure_cache will reallocate if necessary and set cache_cap */
        ensure_cache(n + 1, &cache, &cache_cap);
    }

    /* initialise the first element if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If the required value is already in the cache, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            /* already set */
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t maxk = i / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t nmk = i - k;      /* n - k */
            idx_t kk   = 2 * k;     /* 2*k */
            seq_t comb = binomial(nmk, kk);
            seq_t term = comb * cache[k];
            sum += term;
        }
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
