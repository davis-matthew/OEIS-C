/*
 * A274755  Repunits with even indices multiplied by 99, i.e., 99*(11, 1111,
 *          111111, 11111111, ...).
 * https://oeis.org/A274755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*(10^(2*n) - 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* outside the defined offset */

    /* 10^(2*n) */
    seq_t p = pow10(n << 1);            /* n << 1 == 2*n */
    p -= (seq_t)1;                     /* 10^(2*n) - 1 */
    p *= (seq_t)11;                    /* multiply by 11 */

    return p;
}
