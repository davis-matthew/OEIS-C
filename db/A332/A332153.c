/*
 * A332153  a(n) = 5*(10^(2*n+1)-1)/9 - 2*10^n.
 * https://oeis.org/A332153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 - 2*10^n
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
    seq_t top = pow10(exp);      /* 10^(2*n+1) */
    top = top - 1;               /* 10^(2*n+1) - 1 */
    top = top / 9;               /* divide by 9 */
    top = top * 5;               /* times 5 */

    seq_t sub = pow10(n);        /* 10^n */
    sub = sub * 2;               /* 2 * 10^n */

    return top - sub;            /* final result */
}
