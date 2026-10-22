/*
 * A356568  a(n) = (4^n - 1)*n^(2*n).
 * https://oeis.org/A356568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n - 1) * n^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* 4^0 - 1 = 0, 0^(0) = 1, product is 0 */
    seq_t pow4   = big_pow((seq_t)4, (seq_t)n);          /* 4^n          */
    seq_t pow_n  = big_pow((seq_t)n, (seq_t)n * (seq_t)2);/* n^(2n)       */
    seq_t sub    = pow4 - 1;                           /* 4^n - 1      */
    return sub * pow_n;                               /* final product */
}
