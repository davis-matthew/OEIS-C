/*
 * A367592  Expansion of 1/((1-x) * (1-4*x)^3).
 * https://oeis.org/A367592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9*n^2 + 21*n + 14) * 4^(n+1) - 2) / 54 (OEIS A367592)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once */
    seq_t nn = (seq_t)n;

    /* Compute the polynomial part: 9*n^2 + 21*n + 14 */
    seq_t term = (seq_t)9 * nn * nn + (seq_t)21 * nn + (seq_t)14;

    /* 4^(n+1) = 2^(2n+2) – use the utility pow2 for fast exponentiation */
    seq_t pw = pow2(((idx_t)2 * n) + 2);

    /* Assemble numerator and divide */
    seq_t num = term * pw - (seq_t)2;
    seq_t res = num / (seq_t)54;

    return res;
}
