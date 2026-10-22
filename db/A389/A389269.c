/*
 * A389269  Number of axis-aligned 2 X 2 squares in the 3D projection of a
 *          (3n+1)-dimensional hypercube with side length 2.
 * https://oeis.org/A389269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(3*n+1) - 5*2^(2*n+1) + 2^(n+2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute the three powers of two */
    seq_t t1 = pow2(3*n + 1);   /* 2^(3n+1) */
    seq_t t2 = pow2(2*n + 1);   /* 2^(2n+1) */
    seq_t t3 = pow2(n + 2);     /* 2^(n+2) */

    /* combine according to the formula */
    seq_t result = 3 * t1;      /* 3*2^(3n+1) */
    result -= 5 * t2;           /* subtract 5*2^(2n+1) */
    result += t3;               /* add 2^(n+2) */

    return result;
}
