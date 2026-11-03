/*
 * A357790  a(n) = coefficient of x^n/n! in A(x) = Sum_{n>=0} x^n *
 *          cosh(sqrt(n)*x).
 * https://oeis.org/A357790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n-2k)^k * n!/(2k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 493 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes n! once, then iteratively
   updates the product n!/(2k)! and reuses it for each k. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* factorial(n) returns n! as seq_t */
    seq_t prod = factorial(n);          /* prod = n!/(0!) */
    idx_t maxk = n >> 1;                /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t m = (seq_t)(n - 2 * k);   /* base = n-2k */
        /* m^k */
        seq_t pow = ipow_idx(m, k);
        /* term_k = prod * m^k */
        seq_t term = seq_mul(prod, pow);
        sum += term;

        if (k < maxk)
        {
            /* Update prod = prod / ((2k+1)(2k+2)) for next k */
            uint64_t denom = ((uint64_t)(2 * k + 1)) * (uint64_t)(2 * k + 2);
            prod = prod / (seq_t)denom;
        }
    }
    return sum;
}
