/*
 * A326310  Arithmetic numbers (A003601) that are not RMS numbers (A140480).
 * https://oeis.org/A326310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Implements A326310 – the nth number whose arithmetic mean of
 *            divisors is an integer but whose quadratic mean is not an
 *            integer. The implementation uses a simple sieve-like generator
 *            that tests each integer consecutively and memoises the results
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 1562, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, floor_sqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions supplied by the build environment. */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/pow_uint.h"     /* pow_uint(uint64_t, idx_t) -> seq_t */
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) -> seq_t */

/* Memoisation storage */
static seq_t   *values = NULL;   /* array of numbers that satisfy the condition */
static size_t  length  = 0;      /* number of stored terms   */
static size_t  capacity= 0;      /* allocated capacity      */
static uint64_t current = 2;     /* next integer to test    */

/* Resize and append a new term. */
static void append(seq_t v)
{
    if (length == capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        values = tmp;
        capacity = new_cap;
    }
    values[length++] = v;
}

/* Test if a given m satisfies the definition. */
static bool qualifies(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t  cnt;
    factor_u64(m, pr, ex, &cnt);

    seq_t tau    = 1;   /* divisor count τ(m) */
    seq_t sigma  = 1;   /* sum of divisors σ(m) */
    seq_t sigma2 = 1;   /* sum of squares σ₂(m) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        tau *= (e + 1);

        /* σ(p^e) = (p^(e+1) – 1) / (p – 1) */
        seq_t p_pow_e1   = pow_uint(p, e + 1);
        seq_t term_sigma = (p_pow_e1 - 1) / (((seq_t)p) - 1);
        sigma *= term_sigma;

        /* σ₂(p^e) = (p^(2(e+1)) – 1) / (p² – 1) */
        seq_t p_pow_2e1   = pow_uint(p, 2 * (e + 1));
        seq_t term_sigma2 = (p_pow_2e1 - 1) / ((((seq_t)p) * (seq_t)p) - 1);
        sigma2 *= term_sigma2;
    }

    /* Arithmetic mean integer ⇔ σ(m) divisible by τ(m). */
    if (sigma % tau != 0)
        return false;

    /* Q(m) integer ⇔ σ₂(m)/τ(m) is a perfect square. */
    if (sigma2 % tau == 0) {
        seq_t q2 = sigma2 / tau;
        seq_t root = floor_sqrt(q2);
        return (root * root != q2);
    }

    /* Non‑integer quotient → Q not an integer. */
    return true;
}

/* Ensure that at least n terms have been computed. */
static void ensure_terms(idx_t n)
{
    while (length < (size_t)n) {
        if (qualifies(current))
            append((seq_t)current);
        ++current;
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; safe fallback */
    ensure_terms(n);
    return values[(size_t)n - 1];
}
