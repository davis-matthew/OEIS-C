/*
 * A332123  a(n) = 2*(10^(2n+1)-1)/9 + 10^n.
 * https://oeis.org/A332123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^(2n+1)-1)/9 + 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    idx_t e = 2*n + 1;                 /* exponent for the big repunit part */
    seq_t t = pow10(e);                /* 10^(2n+1) */
    t = t - 1;                         /* 10^(2n+1) - 1 */
    t = t / 9;                         /* (10^(2n+1)-1)/9  (repunit of length 2n+1) */
    t = t * 2;                         /* 2 * repunit */
    seq_t t2 = pow10(n);               /* 10^n */
    return t + t2;                     /* a(n) */
}
