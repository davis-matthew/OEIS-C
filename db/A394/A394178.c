/*
 * A394178  Triangle read by rows: T(n, k) is the Frobenius number for n and k
 *          if n and k coprime, and 0 otherwise, for 1 <= k <= n.
 * https://oeis.org/A394178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 89, i16 n <= 9942, i32 n <= 9942, i64 n <= 9942,
 *            i128 n <= 9942, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Frobenius number of (row, col) if row and col are coprime, otherwise 0
 * where row is the smallest integer with row(row+1)/2 ≥ n and
 * col = n – (row-1)row/2.  For row = 1 and col = 1 this gives -1. */

#include <stdint.h>
#include "../utils/gcd.h"

/* Computes a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Find the row (r) such that r(r+1)/2 ≥ n.  Since n is at most
       ~10 000 for the required tests, a linear search is fast enough. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Compute column index (k) within that row */
    idx_t prev_sum = ((r - 1) * r) / 2;
    idx_t k = n - prev_sum;

    /* If row and column are not coprime, return 0. */
    if (gcd((seq_t)r, (seq_t)k) != 1)
        return 0;

    /* Frobenius number for coprime pair: n*k - n - k. */
    seq_t val = (seq_t)r * (seq_t)k - (seq_t)r - (seq_t)k;
    return val;
}
