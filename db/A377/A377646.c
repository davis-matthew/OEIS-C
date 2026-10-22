/*
 * A377646  Expansion of e.g.f. (1 + x * (exp(x) - 1))^2.
 * https://oeis.org/A377646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*2^(n - 2) - 2*n*(n - 2) for n >= 3; a(0)=1,
 *            a(1)=0, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 4;

    seq_t nn  = (seq_t)n;
    seq_t n1  = nn - 1;
    seq_t n2  = nn - 2;
    seq_t pow = pow2((idx_t)n2);           /* 2^(n-2) */
    seq_t term1 = nn * n1 * pow;
    seq_t term2 = (seq_t)2 * nn * n2;
    return term1 - term2;
}
