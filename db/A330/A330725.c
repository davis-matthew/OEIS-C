/*
 * A330725  a(0) = 0; thereafter a(n) = a(n-1) + sigma(n) if sigma(n) >
 *          a(n-1), otherwise a(n) = a(n-1) - sigma(n), where sigma is the sum
 *          of divisors function A000203.
 * https://oeis.org/A330725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 0; for n > 0: if σ(n) > a(n-1) then a(n) = a(n-1) + σ(n)
 *            else a(n) = a(n-1) - σ(n) where σ(n) is the sum of divisors of n
 * Range:     u8 n <= 59, u16 n <= 9717, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_of.h"

/* memoised cache of the sequence values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)0;

    /* initialise cache if needed */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                    /* out of memory */
        cache[0] = (seq_t)0;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate indices up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache,
                                 (size_t)new_len * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;                        /* out of memory */
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev   = cache[i - 1];
        seq_t sigma  = sigma_of(i);              /* sum of divisors of i */
        if (sigma > prev)
            cache[i] = prev + sigma;
        else
            cache[i] = prev - sigma;
    }
    cache_len = new_len;
    return cache[n];
}
