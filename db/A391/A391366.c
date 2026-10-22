/*
 * A391366  Norms of prime ideals of Z[sqrt(-6)].
 * https://oeis.org/A391366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A391366 – Norms of prime ideals of Z[sqrt(-6)].
 *
 * This implementation generates the terms in increasing order by
 * scanning natural numbers and keeping the ones that satisfy the
 * definition:
 *    - 2 and 3
 *    - primes p ≡ 1,5,7,11 (mod 24)
 *    - squares p² where p is a prime with p ≡ 13,17,19,23 (mod 24)
 *
 * The sequence is memoised, so repeated calls for arbitrary indices
 * are fast.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mul_mod_u64.h"

/* --- utilities ----------------------------------------------------------- */

/* integer square root (floor) for 64‑bit numbers */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = 1ULL << 32;   /* 2^32 > sqrt(2^64-1) */
    while (lo + 1 < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* modular multiplication for 64‑bit numbers (avoids overflow) */

/* modular exponentiation for 64‑bit base and exponent */
static inline uint64_t pow_mod_u64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
    while (d) {
        if (d & 1)
            res = mul_mod_u64(res, a, mod);
        a = mul_mod_u64(a, a, mod);
        d >>= 1;
    }
    return res;
}

/* deterministic Miller–Rabin primality test for 64‑bit integers */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    if (n % 3 == 0)       return n == 3;

    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = pow_mod_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */

/* Sequence entry function */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache   = NULL;   /* cached sequence values        */
    static size_t cache_len = 0;    /* number of cached values       */
    static size_t cache_cap = 0;    /* allocated capacity            */
    static uint64_t next_candidate = 2; /* next number to test          */

    /* grow cache if necessary */
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache = tmp;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        bool add = false;

        if (next_candidate == 2 || next_candidate == 3) {
            add = true;
        } else if (is_prime_u64(next_candidate)) {
            uint8_t r = (uint8_t)(next_candidate % 24);
            if (r == 1 || r == 5 || r == 7 || r == 11)
                add = true;
        } else {
            /* check for squares of primes in the remaining residue classes */
            uint64_t root = isqrt_u64(next_candidate);
            if (root * root == next_candidate && is_prime_u64(root)) {
                uint8_t r = (uint8_t)(root % 24);
                if (r == 13 || r == 17 || r == 19 || r == 23)
                    add = true;
            }
        }

        if (add) {
            /* store the term */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
