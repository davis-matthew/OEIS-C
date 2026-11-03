/*
 * A384992  Positive numbers k whose prime factors together with their
 *          exponents are distinct.
 * https://oeis.org/A384992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384992: numbers whose prime factors and exponents are all distinct */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if k has all distinct prime factors and exponents.  */
static bool is_valid_candidate(uint64_t k)
{
    if (k <= 1)                     /* 1 has no prime factors */
        return true;

    /* For a 64‑bit integer the number of distinct prime factors is ≤ 16. */
    uint64_t seen_primes[16];
    uint64_t seen_exps[16];
    size_t pc = 0, ec = 0;          /* counts of stored primes/exponents */

    uint64_t n = k;

    /* factor the prime 2 first */
    if (!(n & 1ULL)) {
        uint64_t exp = 0;
        while (!(n & 1ULL)) {
            n >>= 1;
            ++exp;
        }
        if (exp == 2)             /* prime equals its exponent → invalid */
            return false;

        /* the first factor, no previous values to compare with */
        seen_primes[pc++] = 2;
        seen_exps[ec++]   = exp;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (exp == p)          /* prime equals its exponent → invalid */
                return false;

            /* check for duplicate values with previously seen primes/exponents */
            for (size_t j = 0; j < pc; ++j) {
                if (p == seen_primes[j]   /* duplicate prime (should not happen) */
                    || exp == seen_primes[j]   /* exponent equals a previous prime */
                    || exp == seen_exps[j])    /* duplicate exponent */
                    return false;
            }
            for (size_t j = 0; j < ec; ++j) {
                if (p == seen_exps[j])   /* prime equals a previous exponent */
                    return false;
            }

            seen_primes[pc++] = p;
            seen_exps[ec++]   = exp;
        }
    }

    /* remaining prime factor greater than sqrt(k) */
    if (n > 1) {
        uint64_t p   = n;
        uint64_t exp = 1;
        if (exp == p)
            return false;

        for (size_t j = 0; j < pc; ++j) {
            if (p == seen_primes[j]
                || exp == seen_primes[j]
                || exp == seen_exps[j])
                return false;
        }
        for (size_t j = 0; j < ec; ++j) {
            if (p == seen_exps[j])
                return false;
        }

        seen_primes[pc++] = p;
        seen_exps[ec++]   = exp;
    }

    return true;
}

/* ----------  cached sequence storage  ---------- */
static seq_t *cache = NULL;          /* dynamic array of computed terms   */
static size_t cache_len = 0;         /* number of terms stored           */
static size_t cache_cap = 0;         /* allocated size of cache          */
static uint64_t last_candidate = 0;  /* last integer tested               */

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t newcap = (cache_cap == 0) ? 1 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_len++] = val;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for index 0 – harmless guard */
        return 0;

    /* compute terms until the required index is reached */
    while (cache_len < (size_t)n) {
        uint64_t candidate = ++last_candidate;
        if (is_valid_candidate(candidate))
            cache_append((seq_t)candidate);
    }

    return cache[(size_t)n - 1];
}
