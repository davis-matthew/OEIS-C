/*
 * A259410  a(n) = 1 - sigma(n) + sigma(n)^2 - sigma(n)^3 + sigma(n)^4.
 * https://oeis.org/A259410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 - sigma(n) + sigma(n)^2 - sigma(n)^3 + sigma(n)^4
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 107, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Cache of primes used to factorise numbers up to a given limit. */
static uint64_t *prime_cache = NULL;      /* array of primes */
static size_t      prime_cnt  = 0;        /* number of primes in cache */
static uint64_t    prime_limit = 0;       /* maximum number up to which primes are cached */

/* Ensure that prime_cache contains all primes <= limit. */
static void
ensure_prime_cache(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    free(prime_cache);
    prime_cache = NULL;
    prime_cnt  = 0;
    build_primes_upto(limit, &prime_cache, &prime_cnt);
    prime_limit = limit;
}

/* Compute the sum of divisors sigma(n) for an unsigned 64‑bit integer n. */
static seq_t
sigma_uint64(uint64_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 1;
    uint64_t temp = n;

    /* All prime factors of n are <= n, so it is enough to cache primes up to n. */
    ensure_prime_cache((uint64_t)n);

    for (size_t i = 0; i < prime_cnt && prime_cache[i] * prime_cache[i] <= temp; ++i) {
        uint64_t p = prime_cache[i];
        if (temp % p == 0) {
            seq_t power = 1;   /* p^0 */
            seq_t sum   = 1;   /* 1 + p + p^2 + ... */
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
                sum   += power;
            }
            result *= sum;
        }
    }

    /* If there is a remaining prime factor > sqrt(n). */
    if (temp > 1) {
        seq_t sum = 1 + (seq_t)temp;   /* 1 + p */
        result *= sum;
    }

    return result;
}

/* External linkage as required. */
seq_t
a(idx_t n)
{
    if (n == 0)            /* undefined for offset 1; safe fallback */
        return 0;

    seq_t s  = sigma_uint64((uint64_t)n);
    seq_t s2 = s * s;          /* sigma^2 */
    seq_t s3 = s2 * s;         /* sigma^3 */
    seq_t s4 = s3 * s;         /* sigma^4 */

    return 1 - s + s2 - s3 + s4;
}
