/*
 * A340660  A000079 is the first row. For the second row, subtract A001045.
 *          For the third row, subtract A001045 from the second one, etc. The
 *          resulting array is read by ascending antidiagonals.
 * https://oeis.org/A340660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = A(i,j) where A(r,c) = 2^c – r·round(2^c/3) and the pair
 *            (i,j) is obtained by reading the infinite array A(r,c) by
 *            ascending antidiagonals. The array element A(r,c) is evaluated
 *            by the closed form 2^c – r * ((2^c + 1) / 3) (since round(2^c/3)
 *            = floor((2^c + 1)/3)
 * Range:     i8 n <= 34, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (((unsigned __int128)(d + 1) * (d + 2)) / 2 <= n)
        d++;

    idx_t terms_before = (d * (d + 1)) / 2;
    idx_t offset      = n - terms_before;          /* 0 <= offset <= d */

    idx_t i = d - offset;  /* row index in the array          */
    idx_t j = offset;      /* column index in the array       */

    seq_t pow2_j   = pow2(j);
    seq_t round_val = (pow2_j + (seq_t)1) / (seq_t)3; /* round(2^j/3) */

    seq_t result = pow2_j - ((seq_t)i * round_val);
    return result;
}
