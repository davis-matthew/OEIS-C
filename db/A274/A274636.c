/*
 * A274636  Square array read by antidiagonals: T(m,n) (m>=0, n>=0) =
 *          (2^floor(m/2)+1)*(2^ceiling(n/2)+1).
 * https://oeis.org/A274636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^floor(m/2)+1) * (2^ceil(k/2)+1) where (m,k) are the
 *            antidiagonal coordinates of n (antidiagonal order:
 *            (0,0),(0,1),(1,0),(0,2),(1,1),(2,0), …)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   antidiagonal_coords, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t col, row;               /* antidiagonal_coords returns col first, row second */
    antidiagonal_coords(n, &col, &row);

    idx_t exp_row = row >> 1;                     /* floor(row/2)   */
    idx_t exp_col = (col >> 1) + (col & 1u);      /* ceil(col/2)    */

    seq_t part_row = pow2(exp_row);
    part_row += (seq_t)1;

    seq_t part_col = pow2(exp_col);
    part_col += (seq_t)1;

    return part_row * part_col;
}
