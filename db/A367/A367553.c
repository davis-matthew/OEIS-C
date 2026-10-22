/*
 * A367553  a(n) = Sum_{d|n} (d+n/d)^n.
 * https://oeis.org/A367553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d + n/d)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Cache of previously computed values */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;

/* Returns the cached value if present, otherwise 0 */
static seq_t cache_lookup(idx_t n)
{
    if (n < cache_capacity)
        return cache[n];
    return (seq_t)0;
}

/* Stores a computed value in the cache (reallocating if needed) */
static void cache_store(idx_t n, seq_t val)
{
    if ((size_t)n >= cache_capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        /* initialise new entries to 0 */
        for (size_t i = cache_capacity; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_capacity = new_cap;
    }
    cache[n] = val;
}

/* Computes A367553 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* not used, safe */
    seq_t cached = cache_lookup(n);
    if (cached != (seq_t)0)
        return cached;

    seq_t sum = (seq_t)0;
    /* iterate over divisors up to sqrt(n) and count each twice if distinct */
    for (uint64_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;
        uint64_t e = n / d;          /* partner divisor */
        seq_t base = (seq_t)d + (seq_t)e;
        seq_t term = big_pow(base, (seq_t)n);
        sum += term;
        if (d != e)                  /* counterpart divisor gives same term */
            sum += term;
    }

    cache_store(n, sum);
    return sum;
}
