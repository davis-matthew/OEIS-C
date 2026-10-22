/*
 * A323590  Primes p such that 2 is a primitive root modulo p while 8192 is
 *          not.
 * https://oeis.org/A323590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323590: primes p such that 2 is a primitive root modulo p
   and p ≡ 1 (mod 13).  The function caches found terms, so it works
   for any order of calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Static cache data ----------------------------------------------------- */
static uint64_t  _curr = 2;          /* next integer to test for primality   */
static seq_t   * _cache = NULL;      /* array of found terms                 */
static size_t   _cache_cap = 0;      /* allocated capacity                   */
static size_t   _cache_len = 0;      /* number of stored terms                */

/* ---------------------------------------------------------------------- */
/* Test whether 2 is a primitive root modulo the odd prime p.            */
/* ---------------------------------------------------------------------- */
static bool _is_primitive_root_2(uint64_t p)
{
    uint64_t rem = p - 1;                  /* φ(p) = p-1                   */
    for (uint64_t d = 2; d * d <= rem; ++d) {
        if (rem % d == 0) {
            /* d is a prime divisor of p-1 */
            seq_t r = mod_pow((seq_t)2, (idx_t)(p - 1) / d, (seq_t)p);
            if (r == 1) return false;
            while (rem % d == 0) rem /= d; /* skip multiplicity          */
        }
    }
    if (rem > 1) {                          /* remaining prime factor        */
        seq_t r = mod_pow((seq_t)2, (idx_t)(p - 1) / rem, (seq_t)p);
        if (r == 1) return false;
    }
    return true;
}

/* ---------------------------------------------------------------------- */
/* Ensure that at least n terms are stored in _cache.                      */
/* ---------------------------------------------------------------------- */
static void _ensure_terms(idx_t n)
{
    /* Grow cache buffer when needed. */
    if (_cache_cap < 16) {
        _cache_cap = 16;
        _cache = (seq_t *)malloc(_cache_cap * sizeof(seq_t));
    }

    while (_cache_len < (size_t)n) {
        /* Skip numbers that are not prime or do not satisfy the modulus */
        if (is_prime(_curr)) {
            if (_curr % 13 == 1 && _is_primitive_root_2(_curr)) {
                /* Store the prime in the cache. */
                if (_cache_len == _cache_cap) {
                    _cache_cap <<= 1;
                    _cache = (seq_t *)realloc(_cache,
                                              _cache_cap * sizeof(seq_t));
                }
                _cache[_cache_len++] = (seq_t)_curr;
            }
        }
        ++_curr;
    }
}

/* ---------------------------------------------------------------------- */
/* Public OEIS function                                                */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool _initialized = false;
    if (!_initialized) {
        _initialized = true;
    }

    if (n == 0u)            /* undefined offset – return 0 as harmless */
        return (seq_t)0;

    _ensure_terms(n);
    return _cache[n - 1u];
}
