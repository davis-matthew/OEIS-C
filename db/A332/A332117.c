/*
 * A332117  a(n) = (10^(2*n+1) - 1)/9 + 6*10^n.
 * https://oeis.org/A332117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2*n+1) - 1)/9 + 6*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    idx_t exp = 2*n + 1;
    seq_t pow_20n1 = pow10(exp);             /* 10^(2*n+1) */
    seq_t repunit = (pow_20n1 - 1) / 9;       /* (10^(2*n+1)-1)/9  */
    seq_t pow_n = pow10(n);                   /* 10^n             */
    return repunit + ((seq_t)6 * pow_n);      /* repunit + 6*10^n  */
}
