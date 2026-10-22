/*
 * A319597  Number of conjugacy classes for a non-abelian group of order p^3,
 *          where p is prime: a(n) = p^2 + p - 1 where p = prime(n).
 * https://oeis.org/A319597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n^2 + p_n - 1, where p_n is the n‑th prime
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, prime_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime on demand, caching the sieve for speed. */

#include <stdlib.h>
#include "../utils/prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache of primes and its upper limit */
    static uint64_t *prime_cache = NULL;
    static size_t prime_cache_cnt = 0;
    static idx_t prime_cache_limit = 0;

    /* Determine a sufficient upper bound for the n‑th prime. */
    idx_t needed_limit = prime_limit_for_idx(n);

    /* Rebuild the sieve if we need a larger limit. */
    if (needed_limit > prime_cache_limit) {
        if (prime_cache)
            free(prime_cache);
        build_primes_upto(needed_limit, &prime_cache, &prime_cache_cnt);
        prime_cache_limit = needed_limit;
    }

    /* The cache is 0‑indexed; n is 1‑indexed. */
    uint64_t p = prime_cache[n - 1];

    /* Compute p^2 + p - 1 using seq_t arithmetic. */
    seq_t pp = (seq_t)p;
    seq_t res = pp * pp;       /* p^2 */
    res += pp;                 /* + p */
    res -= (seq_t)1;           /* - 1 */
    return res;
}
