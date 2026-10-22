/*
 * A345299  a(n) = Sum_{p|n, p prime} p^pi(p).
 * https://oeis.org/A345299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^pi(p) (pi(p) = number of primes <=
 *            p)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

/* Cached list of primes up to prime_cache_limit inclusive.
   The list is stored in ascending order. */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static uint64_t prime_cache_limit = 0;

/* Ensure prime_cache contains all primes <= limit.  Must be called
   with limit >= 2.  The list is freed and rebuilt if limit increases. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_cache_limit)
        return;
    free(prime_cache);
    /* build_primes_upto allocates a new array; caller must free it. */
    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
    prime_cache_limit = limit;
}

/* Binary search for the index of prime p in prime_cache.
   Returns the zero‑based index if found, or SIZE_MAX if not found
   (which should never happen for primes up to prime_cache_limit). */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cache_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t midv = prime_cache[mid];
        if (midv == p)
            return mid;
        if (midv < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return SIZE_MAX;
}

/* Main sequence function.  Uses the cached prime list and big_pow
   to compute each prime divisor contribution. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    uint64_t N = (uint64_t)n;
    if (N <= 1)
        return 0;

    /* Ensure primes up to N are available (N <= 10000 in typical tests). */
    ensure_primes_up_to(N);

    seq_t sum = 0;
    uint64_t remaining = N;

    /* Factorization by trial division over the cached primes. */
    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint64_t p = prime_cache[i];
        /* Stop when p^2 exceeds the current remainder. */
        if (p * p > remaining)
            break;
        if (remaining % p == 0) {
            /* Distinct prime divisor p found. */
            seq_t term = big_pow((seq_t)p, (seq_t)(i + 1)); /* pi(p) = i+1 */
            sum += term;
            /* Remove all powers of p. */
            do {
                remaining /= p;
            } while (remaining % p == 0);
        }
    }

    /* If a residual prime factor > sqrt(N) remains. */
    if (remaining > 1) {
        size_t idx = prime_index(remaining);
        if (idx != SIZE_MAX) {
            seq_t term = big_pow((seq_t)remaining, (seq_t)(idx + 1));
            sum += term;
        }
    }

    return sum;
}
