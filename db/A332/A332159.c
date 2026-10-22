/*
 * A332159  a(n) = 5*(10^(2*n+1)-1)/9 + 4*10^n.
 * https://oeis.org/A332159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 + 4*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Exponent for the first power of ten: 2*n + 1 */
    idx_t e1 = n * 2 + 1;

    /* 10^(2*n+1) */
    seq_t p1 = pow10(e1);

    /* (10^(2*n+1) - 1) / 9 */
    seq_t t = (p1 - 1) / 9;

    /* 5 * ((10^(2*n+1) - 1) / 9) */
    seq_t part1 = t * 5;

    /* 4 * 10^n */
    seq_t part2 = pow10(n) * 4;

    return part1 + part2;
}
