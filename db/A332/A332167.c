/*
 * A332167  a(n) = 6*(10^(2*n+1)-1)/9 + 10^n.
 * https://oeis.org/A332167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*(10^(2*n+1)-1)/9 + 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute 10^n and 10^(2*n+1) */
    seq_t pow_n     = pow10(n);
    seq_t pow_2n1   = pow10(2*n + 1);

    /* term1 = ((10^(2*n+1)-1) / 9) * 6  */
    seq_t term1 = ((pow_2n1 - (seq_t)1) / (seq_t)9) * (seq_t)6;

    /* a(n) = term1 + 10^n */
    return term1 + pow_n;
}
