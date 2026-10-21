/*
 * A242090  Number of triples (a,b,c) with 0 < a < b < c < p and a + b + c ==
 *          0 mod p, where 2*b < p = prime(n).
 * https://oeis.org/A242090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((prime(n)^2 - 6*prime(n) + 12) / 12)
 * Range:     u8 n <= 16, u16 n <= 154, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

/* Cached primes for the current maximum n requested. */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;

/* Ensure we have at least n primes cached. */
static void ensure_prime_cache(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;                      /* already have enough primes */

    /* Estimate an upper bound for the nth prime. */
    idx_t limit;
    if (n < 6) {
        limit = 15;                  /* covers primes 2,3,5,7,11 */
    } else {
        double nn = (double)n;
        /* p_n < n (ln n + ln ln n) for n >= 6  (Rosser's theorem) */
        limit = (idx_t)(nn * (log(nn) + log(log(nn)))) + 3;
    }

    /* Rebuild the cache with the new limit. */
    free(prime_cache);
    build_primes_upto(limit, &prime_cache, &prime_cnt);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Get the nth prime, expanding the cache if necessary. */
    ensure_prime_cache(n);
    uint64_t p = prime_cache[n - 1];

    /* Compute floor((p^2 - 6p + 12) / 12). */
    unsigned long long p64 = (unsigned long long)p;
    unsigned long long num = p64 * p64 - 6ULL * p64 + 12ULL;
    seq_t result = (seq_t)(num / 12ULL);

    return result;
}
