/*
 * A369523  a(n) = n*(n^2 - 1)!.
 * https://oeis.org/A369523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n^2)! / n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A369523 */

/* The factorial() function from the utilities provides an efficient
 * computation of (n^2)!, which works for both fixed-size and
 * arbitrary-precision integer types.  The result is divided by n
 * (which is guaranteed to be a divisor) to obtain a(n). */
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
    /* Compute (n^2)! first.  The product n*n stays within
     * the 64‑bit range for n up to at least 10,000, which is
     * sufficient for the required tests. */
    idx_t m = n * n;

    /* factorial(m) returns the exact factorial in seq_t. */
    seq_t res = factorial(m);

    /* Divide by n; the division is exact. */
    res /= n;

    return res;
}
