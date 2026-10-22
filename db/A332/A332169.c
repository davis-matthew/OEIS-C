/*
 * A332169  a(n) = 6*(10^(2*n+1)-1)/9 + 3*10^n.
 * https://oeis.org/A332169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*(10^(2*n+1)-1)/9 + 3*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p1 = pow10(2*n + 1);
    p1 = p1 - 1;        /* 10^(2*n+1) - 1 */
    p1 = p1 / 9;        /* divide by 9 (exact) */
    p1 = p1 * 6;        /* multiply by 6 */

    seq_t p2 = pow10(n) * 3; /* 3 * 10^n */

    return p1 + p2;
}
