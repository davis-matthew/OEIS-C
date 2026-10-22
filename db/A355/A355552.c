/*
 * A355552  Number of ways to select 3 or more collinear points from a 4 X n
 *          grid.
 * https://oeis.org/A355552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+2) - 4 - 2*n*(n+1) // H(n) + 3 * floor((n^2 + 2)/3)
 *            // 3*D4(n) + 2 * floor((n^2 + 1)/2) // 2*E(n)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute n^2 as seq_t to avoid overflow of idx_t */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* D4(n) = floor((n^2 + 2)/3) */
    seq_t D4 = (sq + 2) / 3;

    /* E(n)  = floor((n^2 + 1)/2) */
    seq_t E  = (sq + 1) / 2;

    /* H(n)  = 2^(n+2) - 4 - 2*n*(n+1) */
    seq_t pow_val = pow2((idx_t)(n + 2));          /* 2^(n+2) */
    seq_t two_n_n1 = (seq_t)2 * (seq_t)n * (seq_t)(n + 1);
    seq_t H = pow_val - 4 - two_n_n1;

    /* Final result */
    return H + (seq_t)3 * D4 + (seq_t)2 * E;
}
