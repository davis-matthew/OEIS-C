/*
 * A292578  Primes of the form 11*n^2 + 55*n + 43.
 * https://oeis.org/A292578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*(n-1)^2 + 55*(n-1) + 43, provided the value is prime
 *            (OEIS offset 1). The function generates primes of this form on
 *            demand and memoises them. It uses an efficient deterministic
 *            Miller‑Rabin test for 64‑bit integers, which is sufficient for
 *            all sizes expected for n ≤ 10000
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 6462, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* ---------- deterministic Miller‑Rabin for 64‑bit integers ---------- */

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* Write n-1 as d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Deterministic bases for <2^64 */
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;    /* undefined for n < 1, safeguard */

    /* Static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cached = 0;     /* number of stored primes */
    static size_t cap = 0;        /* current capacity of cache array */
    static idx_t next_m = 0;      /* next integer m to test (starting at 0) */

    /* Grow cache until it contains at least n primes */
    while (cached < (size_t)n) {
        /* Compute f(m) = 11*m^2 + 55*m + 43 */
        seq_t m = (seq_t)next_m;
        seq_t val = 11 * (m * m) + 55 * m + 43;

        /* Test primality (value fits in 64‑bit for the ranges we care about) */
        if (is_prime_u64((uint64_t)val)) {
            /* Ensure capacity */
            if (cached >= cap) {
                size_t new_cap = cap ? cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failed */
                cache = tmp;
                cap = new_cap;
            }
            cache[cached++] = val;
        }
        ++next_m;
    }
    return cache[n - 1];
}
