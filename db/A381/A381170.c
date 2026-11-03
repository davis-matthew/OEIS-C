/*
 * A381170  Euler transform of n^2 * A065959(n).
 * https://oeis.org/A381170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; * a(n) = (1/n) * Σ_{k=1..n} σ₃(k²) * a(n−k), for n ≥
 *            1. * (σ₃(m) = sum of the 3rd powers of the divisors of m.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

static seq_t *sigma_cache = NULL;   /* sigma_cache[k] = σ₃(k²) */
static size_t sigma_cap = 0;

static seq_t *a_cache = NULL;       /* a_cache[i] = a(i)       */
static size_t a_cap = 0;

/* Ensure sigma_cache[0..n] are computed. */
static void ensure_sigma(idx_t n)
{
    if (n < sigma_cap)
        return;
    /* Allocate (n+1) slots. */
    seq_t *new_cache = mem_resize(seq_t, sigma_cache, (n + 1));
    if (!new_cache) exit(1);
    sigma_cache = new_cache;
    /* Compute new entries. */
    for (idx_t k = sigma_cap; k <= n; ++k) {
        idx_t m = k * k;                 /* k² fits into idx_t for n ≤ 10000   */
        sigma_cache[k] = sigma_k(m, 3);   /* σ₃(m) */
    }
    sigma_cap = n + 1;
}

/* Ensure a_cache[0..n] are computed. */
static void ensure_a(idx_t n)
{
    if (n < a_cap)
        return;
    /* Allocate (n+1) slots. */
    seq_t *new_cache = mem_resize(seq_t, a_cache, (n + 1));
    if (!new_cache) exit(1);
    a_cache = new_cache;
    /* Initialize a(0) if not already. */
    if (a_cap == 0) {
        a_cache[0] = 1;   /* a(0) = 1 */
        a_cap = 1;
    }
    /* Compute new a values. */
    for (idx_t i = a_cap; i <= n; ++i) {
        ensure_sigma(i);   /* σ₃(1..i) ready. */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += sigma_cache[k] * a_cache[i - k];
        }
        a_cache[i] = sum / (seq_t)i;   /* division is exact. */
    }
    a_cap = n + 1;
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[n];
}
