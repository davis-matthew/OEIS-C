/*
 * A372344  Square array A(n, k), n, k >= 0, read by upwards antidiagonals;
 *          for any n, k >= 0 with respective binary expansions Sum_{i >= 0}
 *          b_i * 2^i and Sum_{i >= 0} c_i * 2^i, A(n, k) = Sum_{i >= 0}
 *          (b_{i+1} * c_i - b_i * c_{i+1}) * 3^i.
 * https://oeis.org/A372344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, tabl
 * Formula:   A(n, k) = Sum_{i>=0} (b_{i+1}*c_i - b_i*c_{i+1}) * 3^i where
 *            b_i, c_i are the binary digits of n and k. a(n) is read from the
 *            square array A(n,k) by upwards antidiagonals
 * Range:     i8 n <= 4687, i16 n <= 8757, i32 n <= 8757, i64 n <= 8757,
 *            i128 n <= 8757, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    idx_t t = d * (d + 1) / 2;   /* triangular number for d */
    idx_t k = n - t;             /* position along the antidiagonal */
    idx_t i = d - k;             /* row index */
    idx_t j = k;                 /* column index */

    seq_t result = 0;
    seq_t pow3   = 1;              /* 3^0 */

    /* Determine number of bits to process (one more than the highest set bit) */
    idx_t max_shift = 0;
    while ((i >> max_shift) || (j >> max_shift))
        ++max_shift;

    for (idx_t shift = 0; shift < max_shift; ++shift) {
        int b0 = (i >> shift)       & 1;   /* b_i   */
        int c0 = (j >> shift)       & 1;   /* c_i   */
        int b1 = (i >> (shift + 1)) & 1;   /* b_{i+1} */
        int c1 = (j >> (shift + 1)) & 1;   /* c_{i+1} */

        int term = b1 * c0 - b0 * c1;      /* -1, 0, or 1 */
        if (term)
            result += (seq_t)term * pow3;

        pow3 *= (seq_t)3;
    }

    return result;
}
