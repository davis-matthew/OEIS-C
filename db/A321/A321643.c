/*
 * A321643  a(n) = 5*2^n - (-1)^n.
 * https://oeis.org/A321643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * 2^n - (-1)^n
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 5 * 2^n first */
    seq_t val = pow2(n) * 5;

    /* Adjust by ±1 depending on the parity of n */
    if (n & 1U) {
        /* n is odd: (-1)^n = -1, so subtracting -1 adds 1 */
        val = val + 1;
    } else {
        /* n is even: (-1)^n = 1, so subtract 1 */
        val = val - 1;
    }

    return val;
}
