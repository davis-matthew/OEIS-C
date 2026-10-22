/*
 * A368119  Array read by ascending antidiagonals. A(n, k) =
 *          Product_{j=0..k-1} (n*j + 1).
 * https://oeis.org/A368119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the array A(i,j) = ∏_{t=0}^{j-1} (i*t + 1) along
 *            ascending antidiagonals; i+n must be counted from row 0, column
 *            0
 * Range:     u8 n <= 31, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal s such that n < (s+1)(s+2)/2.
       The number of terms before diagonal s is T_s = s(s+1)/2. */
    idx_t s = 0;
    idx_t before = 0;
    while (n >= before + s + 1) {
        before += s + 1;
        ++s;
    }
    /* Position within the diagonal (0-based, from the end). */
    idx_t pos = n - before;
    idx_t row = s - pos;   /* row index i  */
    idx_t col = pos;       /* column index j */

    /* Compute the product ∏_{t=0}^{col-1} (row*t + 1). */
    seq_t val = 1;
    for (idx_t t = 0; t < col; ++t)
        val *= ((seq_t)row * (seq_t)t + 1);

    return val;
}
