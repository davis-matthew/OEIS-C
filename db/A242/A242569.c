/*
 * A242569  a(n) = n! - 2*n.
 * https://oeis.org/A242569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! - 2*n
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    seq_t f = factorial(n);       /* compute n! in seq_t */
    seq_t two_n = (seq_t)n * 2;   /* compute 2*n safely in seq_t */
    return f - two_n;             /* n! - 2*n */
}
