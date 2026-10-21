/*
 * A242224  Triangular array T read by rows, T(n, k) =
 *          n*k*(gcd(n,k)+2)/gcd(n,k)^2.
 * https://oeis.org/A242224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = n*k*(gcd(n,k)+2)/gcd(n,k)^2 The sequence a(n) is
 *            obtained by reading the triangular array T(n,k) row by row, left
 *            to right, top to bottom with 1‑based indexes
 * Range:     u8 n <= 20, u16 n <= 5049, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r and column c such that
       the n-th term lies in T(r,c).   */
    idx_t row = 1;
    idx_t cum = 1;          /* cumulative count of terms up to current row */

    while (cum < n) {
        row++;
        cum += row;        /* cum = r(r+1)/2 after loop */
    }

    idx_t prev_cum = cum - row;   /* terms before this row */
    idx_t col = n - prev_cum;     /* column within the row */

    /* Compute T(row,col) = row*col*(g+2)/g^2 where g = gcd(row,col) */
    seq_t r = (seq_t)row;
    seq_t c = (seq_t)col;
    seq_t g = gcd(r, c);           /* uses the helper from utils/gcd.h */

    seq_t numerator   = r * c * (g + 2);   /* safe for the sizes involved */
    seq_t denominator = g * g;
    seq_t result = numerator / denominator;

    return result;
}
