/*
 * A321483  a(n) = 7*2^n + (-1)^n.
 * https://oeis.org/A321483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^n + (-1)^n (offset 0)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = pow_int((seq_t)2, n);
    /* compute 7*2^n */
    seq_t result = pow2 * (seq_t)7;
    /* add or subtract 1 depending on the parity of n */
    if ((n & 1) == 0)
        result += 1;   /* even n: (-1)^n = +1 */
    else
        result -= 1;   /* odd n: (-1)^n = -1 */
    return result;
}
