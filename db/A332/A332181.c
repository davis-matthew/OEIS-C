/*
 * A332181  a(n) = 8*(10^(2n+1)-1)/9 - 7*10^n.
 * https://oeis.org/A332181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*(10^(2n+1)-1)/9 - 7*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n is needed twice */
    seq_t pow_n = pow10(n);

    /* compute exponent 2*n+1 safely */
    idx_t exp_2np1 = n * (idx_t)2 + (idx_t)1;
    seq_t pow_2np1 = pow10(exp_2np1);

    /* first part: ((10^(2n+1)-1)/9)*8  (division is exact) */
    seq_t term1 = ((pow_2np1 - (seq_t)1) / (seq_t)9) * (seq_t)8;

    /* second part: 7*10^n */
    seq_t term2 = (seq_t)7 * pow_n;

    return term1 - term2;
}
