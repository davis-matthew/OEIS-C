/*
 * A344114  a(n) = 2^(n^2) - n!.
 * https://oeis.org/A344114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n^2) - n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 56 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

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

seq_t a(idx_t n)
{
    /* n is 1-based; compute 2^(n^2) - n! */
    idx_t exp = n * n;          /* square fits into uint64_t for all n used */
    seq_t term1 = pow2(exp);    /* 2^(n^2) */
    seq_t term2 = factorial(n); /* n! */
    return term1 - term2;
}
