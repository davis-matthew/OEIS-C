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
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_k.h"

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
    seq_t *new_cache = (seq_t *)realloc(sigma_cache, (n + 1) * sizeof(seq_t));
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
    seq_t *new_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
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
