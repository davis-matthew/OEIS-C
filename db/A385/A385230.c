/*
 * A385230  Odd multiplicative orders of -4 modulo primes.
 * https://oeis.org/A385230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = multiplicative order of -4 modulo the n-th prime p for
 *            which this order is odd (p starts from the first such prime)
 * Range:     u8 n <= 58, u16 n <= 7642, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/* Modular exponentiation using 128‑bit intermediate to avoid overflow. */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Static state for prime generation. */
    static uint64_t last_candidate = 2;   /* next_prime(last_candidate) gives next prime > last_candidate */

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Compute terms until we have at least n of them. */
    while (cache_len < n) {
        /* Get next prime. */
        uint64_t p = next_prime(last_candidate);
        last_candidate = p;

        /* Skip p = 2 because -4 mod 2 = 0 is not invertible. */
        if (p < 3) {
            continue;
        }

        /* Compute base = -4 mod p. */
        uint64_t base = (p >= 4U) ? (p - 4U) : (p + p - 4U);  /* safe for p = 3 */

        /* Start with order = p-1. */
        uint64_t order = p - 1U;

        /* Factor p-1 to reduce the order. */
        uint64_t facs[64];
        uint64_t exps[64];
        size_t cnt;
        factor_u64(p - 1U, facs, exps, &cnt);

        /* For each distinct prime factor, try to reduce the order. */
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t q = facs[i];
            while ((order % q) == 0U) {
                uint64_t new_order = order / q;
                if (modpow64(base, new_order, p) != 1U)
                    break;
                order = new_order;
            }
        }

        /* Keep only odd orders. */
        if ((order & 1U) == 0U) {
            continue;
        }

        /* Append to cache. */
        if (cache_len == cache_cap) {
            cache_cap <<= 1U;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)order;
    }

    return cache[n - 1];
}
