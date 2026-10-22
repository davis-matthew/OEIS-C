/*
 * A359410  Integers d such that the longest possible arithmetic progression
 *          (AP) of primes with common difference d has exactly 6 elements.
 * https://oeis.org/A359410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all positive multiples of 30 that are not divisible by 7
   and do not admit a 7‑term arithmetic progression of primes (starting at 7).
   Each such difference d is exactly a term of OEIS sequence A359410. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"   /* for seq_realloc */

/* Prime sieve; false = composite, true = prime.  0 and 1 are marked composite. */
static uint8_t  *sieve = NULL;
static idx_t     sieve_limit = 0;

/* Compute integer square root using binary search.  Works for any idx_t. */
static idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) / 2;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Grow the prime sieve so that it contains all numbers up to 'limit' inclusive. */
static void ensure_sieve(idx_t limit)
{
    if (sieve_limit >= limit)
        return;

    uint8_t *new_sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (!new_sieve)
        return;          /* out of memory - leave old sieve intact */

    for (idx_t i = 0; i <= limit; ++i)
        new_sieve[i] = 1;
    new_sieve[0] = new_sieve[1] = 0;

    idx_t lim = isqrt(limit);
    for (idx_t p = 2; p <= lim; ++p) {
        if (new_sieve[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                new_sieve[j] = 0;
        }
    }

    free(sieve);
    sieve = new_sieve;
    sieve_limit = limit;
}

/* Test whether d admits a 7‑term prime arithmetic progression
   starting at 7.  Returns true if all 7 terms are prime. */
static bool has7term(idx_t d)
{
    idx_t needed = 7 + 6 * d;          /* largest term to check */
    ensure_sieve(needed);

    idx_t val = 7;
    for (idx_t i = 0; i < 7; ++i) {
        if (!sieve[val])
            return false;
        val += d;
    }
    return true;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for index 0 */
        return 0;

    /* Cache of already generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  next_d   = 30;    /* next candidate difference to test */

    while (cache_len < n) {
        idx_t d = next_d;

        /* d must be a multiple of 30, not divisible by 7, and not a 7‑term AP */
        if ((d % 7) != 0 && !has7term(d)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)      /* allocation failed – abort */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)d;
        }

        next_d += 30;
    }

    return cache[n - 1];
}
