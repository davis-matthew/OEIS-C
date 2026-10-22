/*
 * A334084  Integers m such that only 2 binomial coefficients C(m,k), with
 *          0<=k<=m, are practical numbers.
 * https://oeis.org/A334084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=1, a(2)=3, a(3)=5; for n>=4, a(n)=2^(n-1)-1 (Mersenne
 *            numbers)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    /* n >= 4 */
    seq_t res = pow2(n-1);
    return res - 1;
}
