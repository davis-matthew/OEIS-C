/*
 * A369523  a(n) = n*(n^2 - 1)!.
 * https://oeis.org/A369523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n^2)! / n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A369523 */

/* The factorial() function from the utilities provides an efficient
 * computation of (n^2)!, which works for both fixed-size and
 * arbitrary-precision integer types.  The result is divided by n
 * (which is guaranteed to be a divisor) to obtain a(n). */
#include "../utils/factorial.h"

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
