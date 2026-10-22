/*
 * A332171  a(n) = 7*(10^(2n+1)-1)/9 - 6*10^n.
 * https://oeis.org/A332171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*(10^(2n+1)-1)/9 - 6*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute 10^(2n+1) */
    idx_t k = 2 * n + 1;
    seq_t p10k = pow10(k);          /* 10^(2n+1) */
    seq_t p10n = pow10(n);          /* 10^n     */

    /* (10^(2n+1) - 1) / 9   is always an integer             */
    seq_t t = (p10k - 1) / 9;
    t = t * 7;                     /* 7 * ((10^(2n+1)-1)/9)              */

    seq_t sub = p10n * 6;           /* 6 * 10^n                            */

    return t - sub;                 /* complete formula                    */
}
