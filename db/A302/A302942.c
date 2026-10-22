/*
 * A302942  a(n) = (2^n-1)^2*(2^n + 2).
 * https://oeis.org/A302942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n-1)^2*(2^n+2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n);     /* 2^n */
    seq_t t1      = two_pow - 1; /* 2^n - 1 */
    seq_t t2      = two_pow + 2; /* 2^n + 2 */
    seq_t t1sq    = t1 * t1;     /* (2^n - 1)^2 */
    return t1sq * t2;           /* final product */
}
