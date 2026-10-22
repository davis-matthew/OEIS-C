/*
 * A368045  Triangle read by rows. T(n, k) = (k*(k + 1)*(2*k + 1) + n*(n +
 *          1)*(2*n + 1)) / 6.
 * https://oeis.org/A368045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(n, k) = A000330(k) + A000330(n) where * T(n, k) is read
 *            by rows from the triangle * T(r, c) = (c*(c+1)*(2*c+1) +
 *            r*(r+1)*(2*r+1))/6 * (k is the column index in row n after
 *            flattening the triangle)
 * Range:     u8 n <= 34, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   row_from_flat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t in idx_t */
#include "../utils/row_from_flat.h"

/* Compute sum of squares 0^2+1^2+...+x^2 = x*(x+1)*(2*x+1)/6
 * Uses __int128 to avoid overflow for any x that fits in seq_t. */
static inline seq_t sum_squares(idx_t x)
{
    unsigned __int128 v = x;
    v = v * (v + 1) * (2 * v + 1);
    v /= 6;
    return (seq_t)v;
}

/* External-linked function computing the A368045 sequence element. */
seq_t a(idx_t n)
{
    idx_t r = row_from_flat(n);
    idx_t row_start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - row_start;              /* column index in row r */

    return sum_squares(r) + sum_squares(k);
}
