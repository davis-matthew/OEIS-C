/*
 * A368826  Square array T(n,k) = 3*2^k - n read by ascending antidiagonals.
 * https://oeis.org/A368826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = T(row, col) with T(row, col) = 3*2^col - row read by
 *            ascending antidiagonals: (0,0),(1,0),(0,1),(2,0),(1,1),(0,2),
 * Range:     i8 n <= 26, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find diagonal d and the element offset within that diagonal.
       The diagonal d has d+1 elements and the total number of
       elements before it is tri = d*(d+1)/2.
       We increment d until tri + (d+1) would exceed n. */
    idx_t d = 0;
    idx_t tri = 0;
    while (tri + d + 1 <= n) {
        tri += d + 1;
        ++d;
    }

    /* Position within the diagonal. */
    idx_t offset = n - tri;          /* 0 <= offset <= d */
    idx_t row    = d - offset;       /* row index */
    idx_t col    = offset;           /* column index */

    /* Compute the value: 3*2^col - row. */
    seq_t val = pow2(col);             /* 2^col as seq_t */
    val = val * 3 - (seq_t)row;
    return val;
}
