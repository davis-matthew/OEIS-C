/*
 * A363917  a(n) = Product_{p in Factors(n)} mult(p) * n^mult(p) / p, where
 *          Factors(n) is the integer factorization of n and mult(p) the
 *          multiplicity of the prime factor p.
 * https://oeis.org/A363917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p in Factors(n)} mult(p) * n^mult(p) / p
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   build_primes, ipow, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities required from the framework */
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/ipow.h"
#include "../utils/isqrt.h"

/* Compute a single term k * n^k / p, where k is the multiplicity of prime p in n */
static seq_t _term(idx_t n, idx_t p, int k)
{
    seq_t pow_n = 1;
    for (int i = 0; i < k; ++i)
        pow_n *= (seq_t)n;          /* n^k  */

    seq_t term = pow_n * (seq_t)k;   /* k * n^k */
    term /= (seq_t)p;                /* divide by prime p */
    return term;
}

/* Static cache for primes */
static idx_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

/* Ensure primes up to [limit] are available in prime_cache */
static void _ensure_primes(idx_t limit)
{
    if (limit <= prime_cache_limit)
        return;
    idx_t *p = NULL;
    size_t cnt = 0;
    build_primes(limit, &p, &cnt);  /* build_primes allocates new array */
    free(prime_cache);              /* discard old array if any */
    prime_cache = p;
    prime_cache_cnt = cnt;
    prime_cache_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, but return 0 */
    if (n == 1)
        return (seq_t)1;            /* a(1) = 1 */

    /* Prepare primes up to sqrt(n) */
    idx_t lim = isqrt(n);
    if (lim < 2) lim = 2;           /* minimum limit for primes */
    _ensure_primes(lim);

    idx_t temp = n;
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cache_cnt &&
                     (idx_t)prime_cache[i] * prime_cache[i] <= temp; ++i)
    {
        idx_t p = prime_cache[i];
        if (temp % p == 0)
        {
            int k = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++k;
            }
            result *= _term(n, p, k);
        }
    }

    /* Remaining prime factor > sqrt(n), if any */
    if (temp > 1)
    {
        result *= _term(n, temp, 1);
    }

    return result;
}
