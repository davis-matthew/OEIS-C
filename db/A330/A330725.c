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
 * Range:     u8 n <= 59, u16 n <= 12011, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 80 terms through n = 9717 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
        cache = mem_new(seq_t, 1);
        if (!cache)
            return (seq_t)0;                    /* out of memory */
        cache[0] = (seq_t)0;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate indices up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_len);
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
