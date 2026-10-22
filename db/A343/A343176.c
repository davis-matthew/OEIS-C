/*
 * A343176  a(0)=3; for n > 0, a(n) = 2^(2*n) + 3*2^(n-1) + 1.
 * https://oeis.org/A343176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3; for n>0, a(n)=2^(2*n)+3*2^(n-1)+1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)3;
    }
    seq_t term1 = pow2(2 * n);          /* 2^(2n) = 4^n */
    seq_t part2 = pow2(n - 1);          /* 2^(n-1)   */
    seq_t temp = part2 + part2;         /* 2 * 2^(n-1) */
    part2 += temp;                     /* 3 * 2^(n-1) */
    return term1 + part2 + (seq_t)1;
}
