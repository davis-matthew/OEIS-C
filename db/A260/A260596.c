/*
 * A260596  Array A read by upward antidiagonals in which the entry A(n,k) in
 *          row n and column k is defined by A(n,k) = (8 + (3*floor((4*n +
 *          1)/3) - 2)*4^k)/12, n,k >= 1.
 * https://oeis.org/A260596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260596
   Computed by reading the array A(n,k) defined by
     A(n,k) = (8 + (3*floor((4*n + 1)/3) - 2)*4^k) / 12,
   along upward antidiagonals.  The index n specifies the position in
   this reading order. */

#include <stdbool.h>     /* for bool */

seq_t a(idx_t n)
{
    /* ----------  locate the (row,col) of the n‑th element  --------- */
    /* Find the diagonal number d (1‑based) such that the first d
       diagonals contain at least n elements.  Each diagonal d contains
       d elements, so the cumulative count up to diagonal d is d*(d+1)/2. */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) {
        ++d;
    }

    /* Elements before diagonal d */
    idx_t prev_tri = ((d - 1) * d) / 2;

    /* Position within diagonal: 0‐based offset m */
    idx_t m = n - prev_tri - 1;          /* 0 <= m < d */

    /* Coordinates (row, col) in the array (both 1‑based) */
    idx_t row = d - m;
    idx_t col = m + 1;

    /* ----------  evaluate A(row,col)  ------------------------------ */
    /* Compute f(row) = floor((4*row + 1)/3)  */
    idx_t f = (4 * row + 1) / 3;

    /* Compute 4^col in the appropriate integer type */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < col; ++i) {
        pow4 *= (seq_t)4;
    }

    /* temp = (3*f - 2)   is always positive for row >= 1   */
    seq_t temp = (seq_t)(3 * f - 2);

    /* numerator = temp * 4^col + 8   (exact integer)   */
    seq_t numerator = temp * pow4 + (seq_t)8;

    /* final result = numerator / 12 */
    return numerator / (seq_t)12;
}
