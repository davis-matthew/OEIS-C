/*
 * A332162  a(n) = 6*(10^(2*n+1)-1)/9 - 4*10^n.
 * https://oeis.org/A332162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*(10^(2*n+1)-1)/9 - 4*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n */
    seq_t ten_n = pow10(n);
    /* 10^(2n) = (10^n)^2 */
    seq_t ten_2n = ten_n * ten_n;
    /* 10^(2n+1) = 10^(2n) * 10 */
    seq_t ten_2n1 = ten_2n * (seq_t)10;
    /* (10^(2n+1)-1) / 3  → divisible by 3 */
    seq_t tmp  = ten_2n1 - (seq_t)1;
    seq_t tmp3 = tmp / (seq_t)3;
    /* 6/9 = 2/3, so multiply by 2 */
    seq_t res = tmp3 * (seq_t)2;
    /* subtract 4*10^n */
    res -= ten_n * (seq_t)4;
    return res;
}
