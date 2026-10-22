/*
 * A323576  Primes p such that 2 is a primitive root modulo p while 128 is
 *          not.
 * https://oeis.org/A323576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mul_mod64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that 2 is a primitive root modulo p and p ≡ 1 (mod 7). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mul_mod64.h"

/* 64‑bit modular exponentiation (multiplication handled safely). */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1) result = mul_mod64(b, result, mod);
        b = mul_mod64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Test whether 2 is a primitive root modulo prime p (p is guaranteed prime). */
static bool is_primitive_root_of_2(uint64_t p)
{
    /* Factor p−1.  Enough space: p−1 < 2⁶⁴, thus < 64 prime factors. */
    size_t cnt;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(p - 1, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t q = primes[i];
        /* If 2^((p‑1)/q) ≡ 1 (mod p), 2 is not a primitive root. */
        if (powmod64(2ULL, (p - 1) / q, p) == 1ULL)
            return false;
    }
    return true;
}

/* Cached sequence storage. */
static seq_t *cache = NULL;
static size_t   cap   = 0;
static size_t   count = 0;
static uint64_t last_prime = 2;   /* last prime examined */

/* Return the n‑th term of the sequence (n ≥ 1).  n=0 yields 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (count < (size_t)n)
    {
        /* Find next prime after last_prime. */
        uint64_t cand = next_prime(last_prime);
        last_prime = cand;

        /* Must satisfy p ≡ 1 (mod 7). */
        if ((cand % 7ULL) != 1ULL) continue;

        /* Check primitive root condition for 2. */
        if (!is_primitive_root_of_2(cand)) continue;

        /* Grow cache if necessary. */
        if (count >= cap)
        {
            size_t new_cap = cap ? cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }

        /* Store the prime. */
        cache[count++] = (seq_t)cand;   /* cast fits into seq_t in builds that use this sequence */
    }

    /* Return the requested term (1‑based). */
    return cache[(size_t)n - 1];
}
