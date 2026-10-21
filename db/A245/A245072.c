/*
 * A245072  Primes whose reversal + 1 is not a prime.
 * https://oeis.org/A245072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245072: primes p such that rev(p)+1 is not prime. */
/* The function generates primes in increasing order, tests the
   condition, and stores the results in a static cache. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_mul_uint64.h"

/* ---------- Reverse decimal digits of a 64‑bit integer ---------- */
static idx_t rev_uint64(idx_t x)
{
    idx_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* ---------- Deterministic Miller–Rabin test for 64‑bit integers ---------- */

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    __uint128_t result = 1;
    __uint128_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = result * b % mod;
        b = b * b % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime trial division */
    static const uint64_t small_primes[] = {
        2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull,
        19ull, 23ull, 29ull, 31ull, 37ull, 41ull, 43ull,
        47ull, 53ull, 59ull, 61ull, 67ull, 71ull,
        73ull, 79ull, 83ull, 89ull, 97ull
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(uint64_t); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }
    /* deterministic bases for 64-bit n */
    static const uint64_t bases[] = {
        2ull, 325ull, 9375ull, 28178ull,
        450775ull, 9780504ull, 1795265022ull
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(uint64_t); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool cont = false;
        for (unsigned int r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Cache for the sequence ---------- */
static seq_t *seq_cache = NULL;
static size_t   seq_cache_cnt = 0;
static size_t   seq_cache_cap = 0;
static idx_t    next_candidate = 2;   /* next number to test */

/* ---------- Grow the cache if needed ---------- */
static void cache_grow(size_t needed)
{
    if (seq_cache_cap < needed) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */

    /* fill cache up to n */
    while (seq_cache_cnt < n) {
        /* find next prime */
        idx_t cand = next_candidate;
        while (!is_prime_uint64(cand))
            ++cand;

        /* check rev(cand)+1 */
        idx_t revp = rev_uint64(cand);
        if (!is_prime_uint64(revp + 1ULL)) {
            cache_grow(seq_cache_cnt + 1);
            seq_cache[seq_cache_cnt++] = (seq_t)cand;
        }
        /* advance for next iteration */
        next_candidate = cand + 1;
    }
    return seq_cache[(size_t)n - 1];
}
