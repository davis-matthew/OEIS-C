/*
 * A337826  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k^4 *
 *          a(n-k).
 * https://oeis.org/A337826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = (1/n) * Σ_{k=1}^n binomial(n,k)^2 * k^4 * a(n-k) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
static idx_t cache_len = 0;
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t cache_len = 0;
#endif

/* Ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    if (cache.size() <= n)
        cache.resize(n + 1, 0);
#else
    if (cache_size <= n) {
        size_t new_size = n + 1;
        if (new_size < 16)
            new_size = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }
#endif
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Initialize a(0) if necessary */
    if (cache[0] == 0)
        cache[0] = 1;

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t b = binom(i, k);
            seq_t term = b * b;
            seq_t k2 = (seq_t)k * (seq_t)k;
            seq_t k4 = k2 * k2;
            term = term * k4;
#if defined(__cplusplus)
            term = term * cache[i - k];
#else
            term = term * cache[(size_t)(i - k)];
#endif
            sum += term;
        }
        sum /= (seq_t)i;
#if defined(__cplusplus)
        cache[i] = sum;
#else
        cache[i] = sum;
#endif
    }
    cache_len = n;
    return cache[n];
}
