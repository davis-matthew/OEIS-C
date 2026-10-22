/*
 * A319250  Numbers k such that 24k + 11 and 24k + 13 are a pair of twin
 *          primes in A001122.
 * https://oeis.org/A319250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1557, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A319250: k such that 24k+11 and 24k+13 are twin primes
 * with 2 as a primitive root of each.
 * The function builds the sequence lazily, caching the terms so that
 * any calls to a(idx_t n) remain fast regardless of order.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ----- small helper functions ----- */

/* modular exponentiation (base^exp mod mod) for 64‑bit integers */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    __uint128_t result = 1;
    __uint128_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2, 325, 9375, 28178,
                                     450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (__uint128_t)x * x % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* check whether 2 is a primitive root modulo prime p (p > 2). */
static bool has_primitive_root_2(uint64_t p)
{
    uint64_t m = p - 1;
    uint64_t temp = m;

    /* factor 2 once */
    if (temp % 2 == 0) {
        if (mod_pow_uint64(2, m / 2, p) == 1)
            return false;
        while (temp % 2 == 0)
            temp /= 2;
    }

    /* odd prime factors */
    for (uint64_t q = 3; q * q <= temp; q += 2) {
        if (temp % q == 0) {
            if (mod_pow_uint64(2, m / q, p) == 1)
                return false;
            while (temp % q == 0)
                temp /= q;
        }
    }
    if (temp > 1) {   /* remaining prime factor */
        if (mod_pow_uint64(2, m / temp, p) == 1)
            return false;
    }
    return true;
}

/* ----- cached sequence storage ----- */

static seq_t *cache     = NULL;   /* array of k values */
static size_t cache_len = 0;      /* current number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */
static uint64_t next_k   = 0;     /* next k to test */

/* ensure the cache contains at least n terms */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        uint64_t k = next_k;
        uint64_t p = 24 * k + 11;  /* first member of the twin pair */
        uint64_t q = p + 2;        /* second member */

        if (is_prime_uint64(p) && is_prime_uint64(q) &&
            has_primitive_root_2(p) && has_primitive_root_2(q)) {

            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache,
                                 cache_cap * sizeof(seq_t));
                if (!tmp) { free(cache); abort(); }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;
    }
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* below offset; behaviour irrelevant */

    ensure_terms(n);
    return cache[n - 1];
}
