/*
 * A352602  a(n) = 4^n*(2^(2*n+1)-1)*(2*n)!.
 * https://oeis.org/A352602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n * (2^(2*n+1)-1) * (2*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed directly with integer arithmetic. */

#include <stdint.h>
#include "../utils/ipow_i32.h"

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

/* Computes the n-th term of sequence A352602. */
seq_t a(idx_t n)
{
    /* 4^n */
    seq_t p = ipow_i32((seq_t)4, (int)n);

    /* 2^(2*n+1)-1 = 2*4^n-1 */
    seq_t term = (seq_t)2 * p - (seq_t)1;

    /* (2*n)!  */
    seq_t fac = factorial((idx_t)(2 * n));

    /* Final product */
    return p * term * fac;
}
