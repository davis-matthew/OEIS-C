/*
 * A271216  a(n) = 2^n * floor(n/2)!
 * https://oeis.org/A271216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * floor(n/2)!
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 475 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
    idx_t half = n >> 1;            /* floor(n/2) */
    seq_t p2   = pow2(n);           /* 2^n */
    seq_t f    = factorial(half);   /* floor(n/2)! */
    return p2 * f;
}
