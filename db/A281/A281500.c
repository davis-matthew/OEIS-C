/*
 * A281500  Reduced denominators of f(n) = (n+1)/(2^(2+n)-2) with A026741(n+1)
 *          as numerators.
 * https://oeis.org/A281500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + (-1)^n)*(2^(n + 1) - 1)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternate: a(2k) = 4^(k+1) - 2,  a(2k+1) = 4^(k+1) - 1 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return pow2(n + 2) - (seq_t)2;
    else
        return pow2(n + 1) - (seq_t)1;
}
