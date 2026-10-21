/*
 * A291779  a(n) = 2^n - 2^floor(2n/3).
 * https://oeis.org/A291779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2^floor(2n/3)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute floor(2n/3) using integer arithmetic on idx_t */
    idx_t k = (2 * n) / 3;
    seq_t t1 = pow2(n);   /* 2^n */
    seq_t t2 = pow2(k);   /* 2^floor(2n/3) */
    return t1 - t2;       /* subtraction is well defined for seq_t */
}
