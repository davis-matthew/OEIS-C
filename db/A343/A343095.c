/*
 * A343095  Array read by antidiagonals: T(n,k) is the number of k-colorings
 *          of an n X n grid, up to rotational symmetry.
 * https://oeis.org/A343095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where row,col are the coordinates of the nth
 *            element when the infinite table of T(n,k) is read by
 *            antidiagonals. The table entry T(n,k) is defined as: T(n,k) =
 *            (k^(n^2) + 2*k^((n^2 + 3*(n mod 2))/4) + k^((n^2 + (n mod
 *            2))/2))/4. This implementation computes the coordinates via a...
 * Range:     u8 n <= 17, u16 n <= 24, u32 n <= 39, u64 n <= 60, u128 n <= 98,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static void index_to_coords(idx_t n, idx_t *row, idx_t *col)
{
    /* Find the antidiagonal d such that
       t(d) = d*(d+1)/2 <= n < t(d+1) = (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        d++;

    idx_t prev = d * (d + 1) / 2;
    idx_t offset = n - prev;
    *row = offset;
    *col = d - offset;
}

seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_coords(n, &row, &col);

    /* Convert to seq_t for arithmetic. */
    seq_t nrow = (seq_t)row;
    seq_t colval = (seq_t)col;

    seq_t row_sq = nrow * nrow;
    seq_t exp1 = row_sq;
    seq_t exp2 = (row_sq + (seq_t)(3 * (row & 1))) / 4;
    seq_t exp3 = (row_sq + (seq_t)(row & 1)) / 2;

    seq_t term1 = big_pow(colval, exp1);
    seq_t term2 = big_pow(colval, exp2) * (seq_t)2;
    seq_t term3 = big_pow(colval, exp3);

    seq_t sum = term1 + term2 + term3;
    return sum / (seq_t)4;
}
