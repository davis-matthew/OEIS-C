/*
 * A244331  Number of binary digits in the high-water marks of the terms of
 *          the continued fraction of the base-2 Champernowne constant.
 * https://oeis.org/A244331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   a(1)=0, a(2)=1, a(3)=3; for n>=4, a(n) = 2^n - 2*n + 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    seq_t val = pow2(n);                /* 2^n */
    val -= (seq_t)2 * (seq_t)n;         /* subtract 2*n */
    val += (seq_t)1;                    /* add 1 */
    return val;
}
