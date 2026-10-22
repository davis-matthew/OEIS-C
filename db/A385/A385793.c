/*
 * A385793  Primes having only {4, 5, 9} as digits.
 * https://oeis.org/A385793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Primes whose decimal representation uses only the digits 4, 5, 9
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 1334, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_mul64.h"

/* Pre‑computed powers of 3 up to 3^20 – sufficient for n up to 10 000. */
static const uint64_t pow3[21] = {
    1ULL, 3ULL, 9ULL, 27ULL, 81ULL, 243ULL,
    729ULL, 2187ULL, 6561ULL, 19683ULL,
    59049ULL, 177147ULL, 531441ULL, 1594323ULL,
    4782969ULL, 14348907ULL, 43046721ULL,
    129140163ULL, 387420489ULL, 1162261467ULL
};

/* Montgomery‑style modular multiplication (64‑bit). */

/* Modular exponentiation (64‑bit). */
static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t res = 1;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1) res = mod_mul64(res, base, mod);
        base = mod_mul64(base, base, mod);
        e >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Static cache of discovered primes and state for the generator. */
static seq_t *prime_cache = NULL;
static size_t prime_cap   = 0;
static size_t prime_len   = 0;
static size_t cur_len     = 1;          /* current digit length */
static uint64_t cur_idx   = 0;          /* index of next candidate within current length */

/* Public function required by the build. */
seq_t a(idx_t n) {
    /* Ensure n>=1 – undefined behaviour otherwise; we simply return 0. */
    if (n == 0) return (seq_t)0;

    /* Generate primes until we have at least n of them. */
    while (prime_len < (size_t)n) {
        /* Move to the next length once all candidates of the current
           length have been processed. */
        if (cur_idx >= pow3[cur_len]) {
            ++cur_len;
            cur_idx = 0;
            continue;
        }

        /* Build the next candidate of current length from its base‑3 index. */
        uint64_t idx = cur_idx;
        uint64_t candidate = 0;
        for (size_t pos = 0; pos < cur_len; ++pos) {
            uint64_t div = pow3[cur_len - 1 - pos];
            uint64_t code = idx / div;          /* 0 → 4, 1 → 5, 2 → 9 */
            idx %= div;
            uint64_t digit = (code == 0 ? 4 : (code == 1 ? 5 : 9));
            candidate = candidate * 10 + digit;
        }

        /* Check primality using the 64‑bit deterministic test. */
        if (is_prime_uint64((uint64_t)candidate)) {
            /* Grow the cache if needed. */
            if (prime_len == prime_cap) {
                size_t new_cap = prime_cap == 0 ? 32 : prime_cap * 2;
                seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1); /* unrecoverable failure */
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_len++] = (seq_t)candidate;
        }

        /* Advance to the next candidate. */
        ++cur_idx;
    }

    /* Return the required prime. */
    return prime_cache[n - 1];
}
