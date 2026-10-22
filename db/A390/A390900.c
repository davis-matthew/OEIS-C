/*
 * A390900  a(n) = (1/4) * Sum_{k>=0} (3/4)^k * |Stirling1(n+k,k)|.
 * https://oeis.org/A390900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -4*a(n-1) + 8*Sum_{k=0}^{n-1} binomial(n+1, k+1) * a(k) *
 *            a(n-1-k); a(0) = 1; This implementation uses the given
 *            recurrence and memoises all intermediate values. It works for
 *            any integer type seq_t and idx_t as required for the build
 *            system
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, memcpy */
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) returns seq_t */

seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *cache      = NULL;   /* array of computed a(i) */
    static idx_t  cache_used = 0;      /* number of elements stored in cache */

    /* Ensure there is memory for at least (n+1) entries */
    if (cache_used <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* memory allocation failed */

        /* Copy existing values */
        if (cache_used > 0)
            memcpy(new_cache, cache, cache_used * sizeof(seq_t));

        free(cache);
        cache = new_cache;
        /* cache_used will be updated below after we compute missing terms */
    }

    /* Initialisation of the first term */
    if (cache_used == 0) {
        cache[0] = (seq_t)1;
        cache_used = 1;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_used; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t c   = binomial(i + 1, k + 1);   /* C(i+1, k+1) */
            seq_t prod = c * cache[k] * cache[i - 1 - k];
            sum += prod;
        }
        /* a(i) = 8*sum - 4*a(i-1) */
        seq_t val = sum * (seq_t)8;
        val -= cache[i - 1] * (seq_t)4;
        cache[i] = val;
    }
    cache_used = n + 1;   /* update memoised range */

    return cache[n];
}
