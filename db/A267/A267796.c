/*
 * A267796  a(n) = (n+1)*4^(2n+1).
 * https://oeis.org/A267796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*4^(2n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    const seq_t nn  = (seq_t)n;               /* n in seq_t   */
    const seq_t mul = nn + 1;                 /* (n+1)        */
    const seq_t exp = (seq_t)(2*n + 1);       /* 2n+1         */
    const seq_t pow4 = big_pow((seq_t)4, exp);/* 4^(2n+1)     */
    return mul * pow4;                       /* (n+1)*4^(2n+1) */
}
