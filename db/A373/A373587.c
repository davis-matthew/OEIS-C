/*
 * A373587  Numbers k for which A373149(k) is odd, where A373149 is fully
 *          additive with a(2) = 1 and a(p) = prevprime(p) for odd primes p.
 * https://oeis.org/A373587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9718 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth integer k with (A001222(k)+A007949(k)) mod 2 = 1 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ---------- parity test for a positive integer ------------------- */
static bool odd_parity(uint64_t x)
{
    /* exponent of 3 (v3) */
    unsigned v3 = 0;
    while ((x % 3) == 0) {
        ++v3;
        x /= 3;
    }

    /* count remaining prime factors (omega minus v3) */
    unsigned omega_rest = 0;
    uint64_t t = x;
    for (uint64_t p = 2; p * p <= t; ++p) {
        while (t % p == 0) {
            ++omega_rest;
            t /= p;
        }
    }
    if (t > 1)   /* a prime factor > sqrt(x) */
        ++omega_rest;

    unsigned parity_omega = (v3 + omega_rest) & 1U;
    unsigned parity_v3   = v3 & 1U;
    return parity_omega != parity_v3;  /* parity difference? */
}

/* External types supplied by the build system */
typedef uint64_t idx_t;   /* guaranteed unsigned */

/* ---------- dynamic cache for the sequence ---------------------- */
static seq_t *cache          = NULL;   /* pointer to stored terms */
static size_t cache_len      = 0;      /* number already stored */
static size_t cache_cap      = 0;      /* allocated capacity */

/* grow the cache to at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    if (new_cap < needed)
        new_cap = needed;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* the next integer to test for inclusion in the sequence */
static uint64_t test_int = 2;

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for the offset 1, return 0 as a safe default */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    if (cache_len < n) {
        ensure_cache_cap(n);
        while (cache_len < n) {
            /* skip integers that do not satisfy the parity condition */
            while (!odd_parity(test_int))
                ++test_int;

            /* store the valid integer in the cache */
            cache[cache_len++] = (seq_t)test_int;
            ++test_int;        /* next candidate */
        }
    }
    return cache[n - 1];
}
