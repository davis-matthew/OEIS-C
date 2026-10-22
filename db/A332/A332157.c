/*
 * A332157  a(n) = 5*(10^(2*n+1)-1)/9 + 2*10^n.
 * https://oeis.org/A332157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 + 2*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t pow_n = pow10(n);                 /* 10^n           */
    idx_t exp = 2 * n + 1;                  /* 2n + 1         */
    seq_t pow_e = pow10(exp);               /* 10^(2n+1)      */
    seq_t term1 = ((pow_e - (seq_t)1) / (seq_t)9) * (seq_t)5;
    seq_t term2 = pow_n * (seq_t)2;
    return term1 + term2;
}
