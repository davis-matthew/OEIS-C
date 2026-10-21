/*
 * A243755  Primes p such that p is a primitive root modulo the next prime p'
 *          and also p' is a primitive root modulo p.
 * https://oeis.org/A243755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A243755: primes p such that p is a primitive root
 * modulo the next prime p' and p' is a primitive root modulo p.
 * The implementation searches successive primes and caches the ones
 * that satisfy the primitive‑root condition. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/append_candidate.h"

/* Static cache for the sequence values.
 * cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* The next prime to start the search from. */
static uint64_t current_prime = 2;

/* Return true iff a is a primitive root modulo prime p. */
static bool is_primitive_root_u64(uint64_t a, uint64_t p)
{
    if (p == 0 || p == 1)
        return false;
    if (p == 2)
        return true;  /* Any odd a is a primitive root mod 2. */

    uint64_t np1 = p - 1;
    /* Prime factors of p-1. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(np1, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t d = primes[i];
        /* If a^((p-1)/d) ≡ 1 (mod p), then a is not a primitive root. */
        if (mod_pow((seq_t)a, np1 / d, (seq_t)p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_n_terms(idx_t n)
{
    while (cache_len < n) {
        uint64_t p = current_prime;
        uint64_t q = next_prime(p);          /* next prime after p */

        /* Test the primitive‑root condition. */
        if (is_primitive_root_u64(p, q) && is_primitive_root_u64(q, p))
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);

        current_prime = q;   /* advance to the next prime for the next call */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)             /* undefined for n==0; return 0 */
        return (seq_t)0;
    ensure_n_terms(n);
    return cache[n - 1];
}
