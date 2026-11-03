/*
 * A306258  a(n) = floor(n^2/4)*n!.
 * https://oeis.org/A306258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/4)*n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 435 (2026-11-01)
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
    /* Compute n! using the provided helper */
    seq_t fact = factorial(n);

    /* Compute floor(n^2/4) in seq_t arithmetic */
    seq_t square = ((seq_t)n * (seq_t)n) / (seq_t)4;

    /* Return the product */
    return fact * square;
}
