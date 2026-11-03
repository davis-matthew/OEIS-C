/*
 * A389211  Numbers k such that A048146(k)+A162296(k) <= 2*k, where A048146 is
 *          the sum of non-unitary divisors of n, and A162296 is the sum of
 *          divisors of n that have a square factor.
 * https://oeis.org/A389211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that * A048146(k)+A162296(k) ≤
 *            2·k. * Here A048146(k) is the sum of the non‑unitary divisors of
 *            k, * A162296(k) is the sum of the divisors of k that contain a
 *            square factor. * The function returns the k-th such integer
 *            (a(1)=1, a(2)=2, …)
 * Range:     u8 n <= 60, u16 n <= 9548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9840 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --------------------------------------------------------------------- */
static bool condition_holds(idx_t k)
{
    /* Factor k and compute σ(k), σ*(k) (unitary sum), and Q(k)=∏(1+p). */
    idx_t temp = k;
    seq_t sigma      = 1;   /* σ(k)  */
    seq_t sigma_star = 1;   /* σ*(k) */
    seq_t Q           = 1;   /* ∏(1+p) */

    /* Handle factor 2 separately. */
    idx_t exp = 0;
    while ((temp & 1) == 0) {      /* temp % 2 == 0 */
        temp >>= 1;
        ++exp;
    }
    if (exp > 0) {
        seq_t p_pow = 1;           /* 2^exp   */
        seq_t sigma_p = 1;         /* 1 + 2 + … + 2^exp */
        for (idx_t e = 0; e < exp; ++e) {
            p_pow *= 2;
            sigma_p += p_pow;
        }
        sigma      *= sigma_p;
        sigma_star *= (p_pow + 1);
        Q          *= (2 + 1);
    }

    /* Odd prime factors. */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p != 0) continue;
        exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        seq_t p_pow = 1;
        seq_t sigma_p = 1;
        for (idx_t e = 0; e < exp; ++e) {
            p_pow *= p;            /* p^(e+1) eventually */
            sigma_p += p_pow;
        }
        sigma      *= sigma_p;
        sigma_star *= (p_pow + 1);
        Q          *= (p + 1);
    }

    /* Remaining prime factor > 1 */
    if (temp > 1) {
        seq_t p = temp;
        seq_t sigma_p = 1 + p;
        sigma      *= sigma_p;
        sigma_star *= (p + 1);
        Q          *= (p + 1);
    }

    /* Test 2·σ(k) − σ*(k) − Q(k) ≤ 2·k. */
    seq_t lhs = sigma * 2;
    lhs -= sigma_star;
    lhs -= Q;
    seq_t rhs = ((seq_t)k) * 2;
    return lhs <= rhs;
}

/* Variants of idx_t and seq_t are provided by the build system. */

/* --------------------------------------------------------------------- */
/*  Helper: test whether the condition holds for a given integer k.      */

/* --------------------------------------------------------------------- */
/*  Main function: a(idx_t n)                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache of the sequence terms. */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0, cache_cap = 0;

    /* Keep track of the next integer to test. */
    static idx_t next_k = 1;

    /* Grow the cache vector when needed. */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < n) {
        /* Test the current k. */
        if (condition_holds(next_k)) {
            /* Add to cache. */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap < 8) ? 8 : cache_cap * 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
