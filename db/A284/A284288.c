/*
 * A284288  Numbers n such that the average of the strong divisors of n is an
 *          integer.
 * https://oeis.org/A284288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9324 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A284288: find the n‑th integer whose strong‑divisor average is an integer.
   Implements an on‑demand search with trial division factorisation. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether an integer n has integer average of its strong divisors. */
static bool strong_avg_integer(uint64_t n)
{
    if (n <= 1) return false;          /* 1 has no strong divisors */

    uint64_t temp = n;
    uint64_t tau = 1;                  /* number of divisors of n */

    __uint128_t sigma = 1;             /* sum of all divisors of n */

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            tau *= (exp + 1);

            /* term = (p^(exp+1) - 1) / (p - 1) */
            __uint128_t power = 1;
            for (unsigned i = 0; i <= exp; ++i) power *= p;
            __uint128_t term = (power - 1) / (p - 1);
            sigma *= term;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        tau *= 2;
        sigma *= (temp + 1);             /* p^2 - 1 / (p - 1) = p + 1 */
    }

    uint64_t countStrong = tau - 1;      /* tau(n) - 1 */
    if (countStrong == 1) /* prime numbers always satisfy the condition */ return true;

    /* (sigma - 1) % countStrong == 0 ? */
    return ((sigma - 1) % (__uint128_t)countStrong) == 0;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_tested = 1;

    /* Grow cache to hold at least n elements. */
    while (cache_len < n) {
        /* Find next integer with integer strong‑divisor average. */
        do {
            ++last_tested;
        } while (!strong_avg_integer(last_tested));

        /* Ensure capacity. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();        /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)last_tested;
    }

    return cache[n - 1];
}
