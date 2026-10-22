/*
 * A346375  a(n) = Sum_{k=0..n} (2^k + 2) * (2^k + 3) / 2.
 * https://oeis.org/A346375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+1) + 7)*(2^(n+1) + 8)/6 - 9 + 3*n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow2_n1 = pow2(n + 1);
    seq_t tmp1 = pow2_n1 + (seq_t)7;
    seq_t tmp2 = pow2_n1 + (seq_t)8;
    seq_t res   = (tmp1 * tmp2) / (seq_t)6;
    res += (seq_t)(n * 3);
    res -= (seq_t)9;
    return res;
}
