/*
 * A362125  Square array T(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where column k is the expansion of 1/(1 - x*(1+x)^k)^k.
 * https://oeis.org/A362125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row , col) where row and col are obtained from the *
 *            antidiagonal index: n = row + col + row*(row+1)/2. * T(row ,
 *            col) = Σ_{j=0..row} binomial(j+col-1 , j) * binomial(col*j ,
 *            row-j) * with the convention T(row,0) = 1 if row==0, otherwise 0
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline seq_t T_row_col(idx_t row, idx_t col)
{
    /* Special case column 0: only (0,0) is 1, all other entries 0. */
    if (col == 0)
        return (row == 0) ? (seq_t)1 : (seq_t)0;

    seq_t res = 0;
    for (idx_t j = 0; j <= row; ++j)
    {
        /* binomial(j+col-1 , j)   */
        seq_t c1 = binomial(j + col - 1, j);
        /* binomial(col*j , row-j) */
        idx_t m = (col * j >= row) ? (row - j) : (idx_t)(-1); /* will skip if m > col*j */
        seq_t c2 = binomial(col * j, row - j);
        res += c1 * c2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal sum s such that
     * (s+1)(s+2)/2 > n >= s(s+1)/2. */
    idx_t tri = 0;   /* count of entries before current s */
    idx_t s   = 0;
    while (tri + s + 1 <= n)
    {
        tri += s + 1;
        ++s;
    }

    idx_t row = n - tri;      /* row index (0..s)   */
    idx_t col = s - row;      /* column index (0..s)*/

    return T_row_col(row, col);
}
