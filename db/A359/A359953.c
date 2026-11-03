/*
 * A359953  a(1) = 0, a(2) = 1. For n >= 3, if the greatest prime dividing n
 *          is greater than the greatest prime dividing n-1, then a(n) =
 *          a(n-1) + 1. Otherwise a(n) = a(n-1) - 1.
 * https://oeis.org/A359953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1)=0, a(2)=1; for n>=3, * a(n)=a(n-1)+1 if largest prime
 *            factor of n > largest prime factor of n-1, * otherwise
 *            a(n)=a(n-1)-1
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_size = 0;

/* Ensure the cache contains values up to max_n (1‑based). */
static void extend_cache(idx_t max_n) {
    if (max_n <= cache_size)
        return;

    /* grow capacity if necessary */
    if (max_n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
        while (new_cap < max_n + 1)
            new_cap <<= 1;
        seq_t *new_cache = mem_resize(seq_t, cache, (size_t)new_cap);
        if (!new_cache) abort(); /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise first two terms if needed */
    if (cache_size < 1) {
        cache[1] = (seq_t)0;
        cache_size = 1;
    }
    if (max_n >= 2 && cache_size < 2) {
        cache[2] = (seq_t)1;
        cache_size = 2;
    }

    /* compute subsequent terms */
    for (idx_t k = cache_size + 1; k <= max_n; ++k) {
        uint64_t gpf_k   = largest_prime_divisor((uint64_t)k);
        uint64_t gpf_km1 = largest_prime_divisor((uint64_t)(k - 1));
        if (gpf_k > gpf_km1)
            cache[k] = cache[k - 1] + (seq_t)1;
        else
            cache[k] = cache[k - 1] - (seq_t)1;
    }
    cache_size = max_n;
}

seq_t a(idx_t n) {
    if (n == 0)  /* undefined for offset 1, but guard anyway */
        return (seq_t)0;
    extend_cache(n);
    return cache[n];
}
