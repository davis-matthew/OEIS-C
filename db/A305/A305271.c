/*
 * A305271  a(n) = 680*2^n - 548.
 * https://oeis.org/A305271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 680*2^n - 548
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p   = pow2(n);              /* 2^n          */
    seq_t prod = (seq_t)680 * p;      /* 680 * 2^n     */
    seq_t res  = prod - (seq_t)548;   /* subtract 548   */
    return res;
}
