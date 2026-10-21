/*
 * A271440  a(n) = sigma(prime(n)^n) - phi(prime(n)^n).
 * https://oeis.org/A271440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(prime(n)^n) - phi(prime(n)^n) = (2*prime(n)^n -
 *            prime(n)^(n-1) - 1) / (prime(n)-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   _prime_limit_for_idx, big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Cached primes up to some index.  They are allocated once and
   extended if needed.  The cache persists across calls to a(). */
static uint64_t *cached_primes = NULL;
static size_t cached_count = 0;

/* Ensure that primes 1..n are available in cached_primes. */
static void ensure_primes(idx_t n) {
    if (cached_count >= (size_t)n)
        return;

    /* Estimate an upper bound for the nth prime. */
    idx_t limit = _prime_limit_for_idx(n);

    /* Allocate new array of primes. */
    size_t count;
    uint64_t *new_primes = NULL;
    build_primes_upto(limit, &new_primes, &count);

    /* Replace the old cache.  The caller does not need to free it. */
    free(cached_primes);
    cached_primes = new_primes;
    cached_count = count;
}

/* Compute a(n) for n >= 1.  Caller must ensure n > 0. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;           /* undefined, but we return 0 for safety */

    /* Make sure we have the first n primes. */
    ensure_primes(n);

    /* Retrieve the nth prime (1-indexed).  Array is 0-indexed. */
    uint64_t p_val = cached_primes[(size_t)(n - 1)];
    seq_t p = (seq_t)p_val;

    /* Compute p^n and p^(n-1). */
    seq_t p_pow_n        = big_pow(p, (seq_t)n);
    seq_t p_pow_n_minus1 = big_pow(p, (seq_t)(n - 1));

    /* Numerator: 2 * p^n - p^(n-1) - 1. */
    seq_t numerator = 2 * p_pow_n - p_pow_n_minus1 - 1;

    /* Denominator: p - 1. */
    seq_t denom = p - 1;

    /* Final result.  Division is exact by the definition. */
    return numerator / denom;
}
