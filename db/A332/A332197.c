/*
 * A332197  a(n) = 10^(2n+1) - 1 - 2*10^n.
 * https://oeis.org/A332197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2n+1) - 1 - 2*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute 10^(2n+1) - 1 - 2*10^n */
    idx_t exp = 2*n + 1;          /* exponent for 10^(2n+1) */
    seq_t pow_big    = pow10(exp);/* 10^(2n+1) */
    seq_t pow_small  = pow10(n);  /* 10^n */
    seq_t result = pow_big - (seq_t)1;     /* subtract 1 */
    result -= ((seq_t)2 * pow_small);     /* subtract 2*10^n */
    return result;
}
