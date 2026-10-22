/*
 * A307562  Numbers k such that both 6*k + 1 and 6*k + 7 are prime.
 * https://oeis.org/A307562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that 6*k+1 and 6*k+7 are prime. The function
 *            returns the n-th such k (1‑based)
 * Range:     u8 n <= 46, u16 n <= 3762, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: modular multiplication (a*b)%mod using 128‑bit arithmetic */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Helper: modular exponentiation (base^exp)%mod */
static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    uint64_t x = base % mod;
    while (exp) {
        if (exp & 1) result = mul_mod(result, x, mod);
        x = mul_mod(x, x, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers.
   Bases {2, 325, 9375, 28178, 450775, 9780504, 1795265022} are sufficient. */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* quick small prime test */
    static const uint64_t small_primes[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL,
        28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Global cache of sequence terms */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;       /* number of terms stored */
static size_t cache_cap = 0;        /* allocated capacity */
static idx_t next_k = 1;            /* next k to test */

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure we have at least n terms cached */
    while (cache_size < n) {
        /* Check 6*k+1 and 6*k+7 for primality */
        uint64_t val1 = (uint64_t)6 * (uint64_t)next_k + 1;
        uint64_t val2 = val1 + 6;          /* 6*k+7 */
        if (is_prime_u64(val1) && is_prime_u64(val2)) {
            /* Store the term */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? (cache_cap << 1) : 128;
                seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
            }
            seq_cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return seq_cache[n - 1];
}
