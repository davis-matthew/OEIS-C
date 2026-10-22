/*
 * A321003  a(n) = 2^n*(4*3^n-1).
 * https://oeis.org/A321003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (4*3^n - 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* 2^n and 3^n computed with efficient exponentiation */
    seq_t two_n  = pow2(n);
    seq_t three_n = pow3(n);

    /* factor = 4 * 3^n - 1 */
    seq_t factor = three_n;
    factor *= 4;
    factor -= 1;

    /* a(n) = (2^n) * factor */
    return two_n * factor;
}
