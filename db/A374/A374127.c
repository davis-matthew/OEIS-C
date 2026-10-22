/*
 * A374127  a(n) = (sigma(n) - sopfr(n)) - n, where sigma is the sum of
 *          divisors, and sopfr is the sum of prime factors (with repetition).
 * https://oeis.org/A374127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) - sopfr(n) - n
 * Range:     i8 n <= 89, i16 n <= 8787, i32 n <= 8787, i64 n <= 8787,
 *            i128 n <= 8787, big any n
 * Helpers:   build_primes_upto, floor_isqrt_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n ≥ 1; uses a cached prime list up to sqrt(n). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_isqrt_u64.h"   /* floor_isqrt_u64(idx_t n)  */
#include "../utils/build_primes_upto.h"/* build_primes_upto(idx_t limit, uint64_t **, size_t *) */
#include "../utils/sigma_of.h"         /* seq_t sigma_of(idx_t n) */

static uint64_t *prime_cache = NULL;  /* cached primes */
static size_t    prime_cnt   = 0;
static idx_t     prime_limit = 0;      /* maximum sqrt processed so far */

/* Expands prime_cache to cover sqrt(n) if necessary */
static void ensure_prime_cache(idx_t n)
{
    idx_t limit = floor_isqrt_u64(n);
    if (limit > prime_limit)
    {
        free(prime_cache);
        build_primes_upto(limit, &prime_cache, &prime_cnt);
        prime_limit = limit;
    }
}

seq_t a(idx_t n)
{
    /* Ensure we have primes up to sqrt(n) for factorisation */
    ensure_prime_cache(n);

    /* Compute sigma(n) directly */
    seq_t sigma = sigma_of(n);

    /* Compute sopfr(n): sum of prime factors with multiplicity */
    uint64_t tmp = (uint64_t)n;
    seq_t   sopfr = 0;
    size_t  i = 0;

    while (i < prime_cnt && (uint64_t)prime_cache[i] <= tmp / prime_cache[i])
    {
        uint64_t p = prime_cache[i];
        uint64_t e = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++e;
        }
        if (e > 0)
        {
            sopfr += (seq_t)(e * p);
        }
        ++i;
    }

    /* Remaining prime factor, if any */
    if (tmp > 1)
    {
        sopfr += (seq_t)tmp;
    }

    seq_t result = sigma - sopfr - (seq_t)n;
    return result;
}
