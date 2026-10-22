/*
 * A342974  Primes p such that the order of 2 modulo p is not divisible by the
 *          largest odd divisor of p - 1.
 * https://oeis.org/A342974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that the multiplicative order of 2
 *            modulo p is NOT divisible by the largest odd divisor of p‑1
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   factor_u64, next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/powmod64.h"

/* Static storage of primes that satisfy the property.  The array
   holds uint64_t values; the function returns them cast to seq_t. */
static uint64_t *prime_cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_checked = 1;   /* last prime examined */

/* Ensure the prime_cache has at least new_cap elements.  The array is
   of uint64_t, which is a trivially copyable type, so realloc is
   safe for both C and C++. */
static void ensure_prime_cache_cap(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                        new_cap * sizeof(uint64_t));
    if (!tmp) abort();                  /* out of memory */
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Test whether a prime p satisfies the sequence condition. */
static bool good_prime(uint64_t p)
{
    if (p <= 2) return false;           /* skip trivial cases */

    /* Compute the order of 2 modulo p. */
    uint64_t ord  = p - 1;               /* p is prime → ord | p-1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(ord, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        while (ord % q == 0 &&
               powmod64(2, ord / q, p) == 1)
            ord /= q;
    }

    /* Largest odd divisor of p-1. */
    uint64_t odd_part = p - 1;
    while ((odd_part & 1) == 0)
        odd_part >>= 1;

    /* Property: ord % odd_part != 0. */
    return (ord % odd_part) != 0;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* safeguard for offsets < 1 */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(last_checked + 1);
        last_checked = p;

        if (good_prime(p)) {
            ensure_prime_cache_cap(cache_len + 1);
            prime_cache[cache_len++] = p;
        }
    }

    return (seq_t)prime_cache[n - 1];
}
