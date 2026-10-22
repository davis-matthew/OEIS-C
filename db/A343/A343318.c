/*
 * A343318  a(n) = (2^n + 1)^3.
 * https://oeis.org/A343318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 1)^3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t t = pow2(n);     /* t = 2^n */
    t = t + 1;             /* t = 2^n + 1 */
    t = t * t * t;         /* t = (2^n + 1)^3 */
    return t;
}
