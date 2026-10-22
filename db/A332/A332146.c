/*
 * A332146  a(n) = 4*(10^(2*n+1)-1)/9 + 2*10^n.
 * https://oeis.org/A332146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*(10^(2*n+1)-1)/9 + 2*10^n (offset 0)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^(2*n+1) */
    seq_t p = pow10(2 * n + 1);

    /* (10^(2*n+1)-1)/9 * 4 */
    seq_t tmp = p - 1;
    tmp = tmp / 9;
    tmp = tmp * 4;

    /* 2*10^n */
    seq_t t3 = pow10(n);
    t3 = t3 * 2;

    return tmp + t3;
}
