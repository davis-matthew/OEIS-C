/*
 * A291009  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S)*(1 - 3*S).
 * https://oeis.org/A291009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * (9*2^n - 1), a(0) = 4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    seq_t two_n  = pow2(n);            /* 2^n */
    seq_t term   = 9 * two_n - (seq_t)1;/* 9*2^n - 1 */
    seq_t result = pow2(n-1) * term;    /* 2^(n-1) * (9*2^n - 1) */
    return result;
}
