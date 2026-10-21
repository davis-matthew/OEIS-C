/*
 * A293213  Primes p with phi(p-1) a primitive root modulo p, where phi(.) is
 *          Euler's totient function (A000010).
 * https://oeis.org/A293213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 2191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293213: primes p such that φ(p-1) is a primitive root modulo p.
   The function searches primes on demand, caching results so that successive
   calls are fast. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;      /* array of qualifying primes */
static size_t cache_len = 0;     /* number of stored primes */
static size_t cache_cap = 0;     /* current allocation size */
static uint64_t last_prime = 0;  /* last prime that has been examined */

/* Test whether a prime p satisfies the condition. */
static bool satisfies(uint64_t p)
{
    if (p == 2)                           /* φ(1)=1 is a primitive root mod 2 */
        return true;

    /* Factor p‑1. */
    uint64_t factors[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(p - 1, factors, exps, &cnt);

    /* Compute φ(p‑1). */
    seq_t phi = p - 1;
    for (size_t i = 0; i < cnt; ++i)
        phi -= phi / factors[i];

    /* Test that φ(p‑1) is a primitive root modulo p. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = factors[i];
        seq_t r = mod_pow(phi, (p - 1) / q, p);
        if (r == 1)
            return false;
    }
    return true;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, but harmless */

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        last_prime = next_prime(last_prime);     /* next prime > last_prime */
        if (satisfies(last_prime)) {
            /* Grow the cache if needed. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = (seq_t)last_prime;
        }
    }

    return cache[n - 1];
}
