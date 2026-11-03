/*
 * A397790  a(n) = prime^2 closest to prime(n)^3.
 * https://oeis.org/A397790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 34, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 50 terms through n = 9881 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = square of the prime closest to prime(n)^3, offset 1. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* A simple dynamic array of primes (1‑based). */
static uint32_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static size_t prime_cache_cap = 0;
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure the cache contains at least n primes. */
static void ensure_primes(idx_t n)
{
    while (prime_cache_cnt < n) {
        if (is_prime_big((seq_t)next_candidate)) {
            if (prime_cache_cnt == prime_cache_cap) {
                size_t new_cap = prime_cache_cap ? prime_cache_cap << 1 : 16;
                uint32_t *tmp = (uint32_t *)realloc(prime_cache,
                                                    new_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_cache = tmp;
                prime_cache_cap = new_cap;
            }
            prime_cache[prime_cache_cnt++] = (uint32_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    ensure_primes(n);

    /* p = prime(n) */
    seq_t p = (seq_t)prime_cache[n - 1];

    /* cube = p^3 */
    seq_t cube = p * p * p;

    /* target ≈ √(p³) */
    seq_t target = floor_sqrt(cube);

    bool has_low = false, has_hi = false;
    seq_t low_sq = 0, hi_sq = 0;
    seq_t low_diff = 0, hi_diff = 0;

    /* Search outward from target until primes on both sides are found. */
    for (seq_t d = 0;; ++d) {
        /* lower candidate (only if not already found) */
        if (!has_low && target >= d) {
            seq_t cand = target - d;
            if (cand >= 2 && is_prime_big(cand)) {
                seq_t sq = cand * cand;
                low_diff = (cube > sq) ? (cube - sq) : (sq - cube);
                low_sq = sq;
                has_low = true;
            }
        }

        /* higher candidate (skip when d==0 to avoid duplicate of target) */
        if (!has_hi && d > 0) {
            seq_t cand = target + d;
            if (cand >= 2 && is_prime_big(cand)) {
                seq_t sq = cand * cand;
                hi_diff = (cube > sq) ? (cube - sq) : (sq - cube);
                hi_sq = sq;
                has_hi = true;
            }
        }

        if (has_low && has_hi) break;
        /* Safety guard (primes exist beyond any reasonable bound). */
        if (d > 2000000) break;
    }

    /* If one side never found (should not happen), return the one that was. */
    if (!has_low) return hi_sq;
    if (!has_hi)  return low_sq;

    /* Return the closer prime square (low if equal). */
    return (low_diff <= hi_diff) ? low_sq : hi_sq;
}
