/*
 * A386337  Primes without {2, 6} as digits.
 * https://oeis.org/A386337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 2587, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_forbidden_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes whose decimal digits do not contain 2 or 6.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/has_forbidden_digit.h"

/* Deterministic Miller‑Rabin primality test for 64‑bit integers.  */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t smallp[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
         23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(smallp)/sizeof(smallp[0]); ++i) {
        uint64_t p = smallp[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
         9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            return true;

        __uint128_t x = 1, p = a, e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        uint64_t r = (uint64_t)x;
        if (r == 1 || r == n - 1)
            continue;

        bool composite = true;
        for (uint64_t j = 1; j < s; ++j) {
            x = (__uint128_t)x * x % n;
            r = (uint64_t)x;
            if (r == n - 1) { composite = false; break; }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Static cache of the sequence. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 3;   /* next odd integer to test */

/* Ensure the cache holds at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* initialise cache once */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) /* allocation failure – best effort */
            return;
        cache_len = 0;
        next_candidate = 3;
    }

    /* grow cache if necessary */
    if (cache_cap < (size_t)n) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        while (newcap < (size_t)n)
            newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = newcap;
        }
    }

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        /* skip numbers containing forbidden digits */
        if (!has_forbidden_digit(next_candidate)) {
            uint64_t cand = (uint64_t)next_candidate;
            if (is_prime_u64(cand)) {
                cache[cache_len++] = (seq_t) cand;
            }
        }
        next_candidate += 2;   /* only odd numbers are relevant */
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* behaviour below offset is irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}
