/*
 * A259098  Row sums of A146565.
 * https://oeis.org/A259098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*2^(n-4) for n >= 5, a(0)=1, a(1)=2, a(2)=3, a(3)=6,
 *            a(4)=12
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)12;

    /* n >= 5 */
    seq_t val = pow2(n - 4);          /* 2^(n-4) */
    val *= (seq_t)13;                 /* 13 * 2^(n-4) */
    return val;
}
