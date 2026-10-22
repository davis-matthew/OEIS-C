/*
 * A391685  Array read by ascending antidiagonals: A(n,k) = (2*n +
 *          1)*2^(2*k+1) - 1 with k >= 0.
 * https://oeis.org/A391685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (2*i + 1) * 2^(2*j + 1) – 1 * where (i,j) are the
 *            antidiagonal coordinates of the linear index n
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal d with length d+1 that contains n. */
    idx_t d   = 0;
    idx_t cum = 0;                     /* terms before diagonal d   */
    while (cum + (d + 1) <= n) {
        cum += d + 1;
        ++d;
    }

    /* Position within that diagonal. */
    idx_t s = n - cum;                 /* 0 … d                       */
    idx_t i = d - s;                   /* row index                   */
    idx_t j = s;                       /* column index                */

    /* Compute 2^(2*j+1) by repeated multiplication (no UB for any j). */
    seq_t pow2 = (seq_t)1;
    for (idx_t k = 0; k < (2 * j + 1); ++k)
        pow2 *= (seq_t)2;

    /* (2*i + 1)*pow2 – 1. */
    seq_t base = (seq_t)2 * (seq_t)i + 1;
    return base * pow2 - 1;
}
