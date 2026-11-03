/*
 * A282822  a(n) = (n - 4)*n! for n>=0.
 * https://oeis.org/A282822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n - 4)*n!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
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
    seq_t seq_n = (seq_t)n;
    seq_t fact = factorial(n);
    return fact * (seq_n - 4);
}
