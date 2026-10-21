/*
 * A291658  a(n) is the sum of all integers from 5^n to 5^(n+1)-1.
 * https://oeis.org/A291658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*5^(2n) - 2*5^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t p = big_pow((seq_t)5, (seq_t)n);  /* p = 5^n */
    seq_t p2 = p * p;                       /* p^2 = 5^(2n) */
    seq_t res = 12 * p2 - 2 * p;            /* 12*5^(2n) - 2*5^n */
    return res;
}
