/*
 * A288443  a(n) = (2n + 1)*2^(2n + 1); numbers k such that v(k)*2^v(k) = k,
 *          where v(n) = A007814(n) is 2-adic valuation of n.
 * https://oeis.org/A288443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n + 1) * 2^(2n + 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sn   = n;                           /* convert the index to seq_t */
    seq_t num  = sn * (seq_t)2 + 1;            /* 2n + 1 */
    idx_t  exp = n * 2 + 1;                    /* 2n + 1 as exponent */
    seq_t den  = pow2(exp);                    /* 2^(2n + 1) */
    return num * den;                         /* product */
}
