/*
 * A248877  a(1) = 23, a(2) = 71, a(n) = 3*a(n-1) - 2*a(n-2) for n>2.
 * https://oeis.org/A248877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3*2^(n+3)-25
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+3) using the utility function */
    seq_t p = pow2(n + 3);
    /* a(n) = 3*p - 25 */
    return p * (seq_t)3 - (seq_t)25;
}
