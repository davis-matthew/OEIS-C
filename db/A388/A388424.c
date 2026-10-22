/*
 * A388424  Number of axis-aligned 2 X 2 squares in the 3D projection of a
 *          (3n+2)-dimensional hypercube with side length 2.
 * https://oeis.org/A388424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*2^(3*n) - 16*2^(2*n) + 5*2^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow2_n  = pow2(n);
    seq_t pow2_2n = pow2(2 * n);
    seq_t pow2_3n = pow2(3 * n);
    return (seq_t)12 * pow2_3n - (seq_t)16 * pow2_2n + (seq_t)5 * pow2_n;
}
