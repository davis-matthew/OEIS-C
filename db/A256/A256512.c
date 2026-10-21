/*
 * A256512  a(n) = n*(1+(2*n)^n).
 * https://oeis.org/A256512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(1+(2*n)^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* (2*0)^0 is defined as 1, so a(0)=0 */
    seq_t base = (seq_t)n * (seq_t)2;   /* base = 2*n */
    seq_t pow  = big_pow(base, (seq_t)n); /* pow = (2*n)^n */
    seq_t one  = 1;
    /* a(n) = n * (1 + pow) */
    return (seq_t)n * (one + pow);
}
