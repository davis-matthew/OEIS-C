/*
 * A384490  Prime numbers p such that both roots of x^2 - x - 1 modulo p are
 *          primitive roots modulo p.
 * https://oeis.org/A384490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   OEIS sequence A384490: primes p such that both roots of x^2 - x
 *            - 1 (mod p) are primitive roots modulo p. Implementation:
 *            brute‑force search over primes
 * Range:     u8 n <= 5, u16 n <= 589, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, mul_mod64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/append_candidate.h"
#include "../utils/mul_mod64.h"

/* 64‑bit modular multiplication using 128‑bit intermediate. */

/* 64‑bit modular exponentiation (binary). */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Tonelli–Shanks: modular square root for odd prime modulus. */
static uint64_t mod_sqrt(uint64_t n, uint64_t p)
{
    if (n == 0) return 0;
    if (p == 2) return n & 1;
    if (modpow64(n, (p - 1) >> 1, p) != 1) return 0;   /* not a residue */
    if (p % 4 == 3) return modpow64(n, (p + 1) >> 2, p);

    uint64_t q = p - 1, s = 0;
    while ((q & 1) == 0) { q >>= 1; ++s; }

    uint64_t z = 2;
    while (modpow64(z, (p - 1) >> 1, p) != p - 1) ++z;

    uint64_t c = modpow64(z, q, p);
    uint64_t x = modpow64(n, (q + 1) >> 1, p);
    uint64_t t = modpow64(n, q, p);
    uint64_t m = s;

    while (t != 1) {
        uint64_t i = 1;
        uint64_t t2i = modpow64(t, 2, p);
        while (t2i != 1) {
            t2i = modpow64(t2i, 2, p);
            ++i;
        }
        uint64_t b = modpow64(c, ((uint64_t)1 << (m - i - 1)), p);
        x = mul_mod64(x, b, p);
        b = mul_mod64(b, b, p);
        t = mul_mod64(t, b, p);
        c = b;
        m = i;
    }
    return x;
}

/* Small‑prime sieve for factorisation of p-1. */
static uint32_t *small_primes = NULL;
static size_t small_prime_cnt = 0;
static void init_small_primes(void)
{
    if (small_primes != NULL) return;
    const uint32_t LIMIT = 2000000U;
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++small_prime_cnt;
    small_primes = (uint32_t*)malloc(small_prime_cnt * sizeof(uint32_t));
    size_t k = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) small_primes[k++] = i;
    free(is_comp);
}

/* Test whether r is a primitive root modulo prime p. */
static bool is_primitive_root(uint64_t p, uint64_t r)
{
    uint64_t phi = p - 1;
    uint64_t tmp = phi;
    for (size_t i = 0; i < small_prime_cnt && (uint64_t)small_primes[i] * small_primes[i] <= tmp; ++i) {
        uint64_t q = small_primes[i];
        if (tmp % q == 0) {
            while (tmp % q == 0) tmp /= q;
            if (modpow64(r, phi / q, p) == 1)
                return false;
        }
    }
    if (tmp > 1) {
        if (modpow64(r, phi / tmp, p) == 1)
            return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 2;

    if (!initialized) {
        init_small_primes();
        initialized = true;
    }

    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(last_checked);
        last_checked = p;
        if (p < 7) continue;   /* skip very small primes */

        /* 5 must be a quadratic residue mod p. */
        if (modpow64(5, (p - 1) >> 1, p) != 1)
            continue;

        uint64_t sqrt5 = mod_sqrt(5, p);
        if (sqrt5 == 0) continue;
        uint64_t inv2 = (p + 1) >> 1;   /* 2^{-1} mod p */

        uint64_t r1 = mul_mod64((1ULL + sqrt5) % p, inv2, p);
        if (!is_primitive_root(p, r1))
            continue;
        uint64_t r2 = mul_mod64((1ULL + p - sqrt5) % p, inv2, p);
        if (!is_primitive_root(p, r2))
            continue;

        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);
    }

    return cache[n - 1];
}
