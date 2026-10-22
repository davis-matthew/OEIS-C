/*
 * A395948  Expansion of e.g.f. sinh(x)^2*cosh(x)*exp(x).
 * https://oeis.org/A395948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) * (2^(n-1) - (n mod 2)), n > 0; a(0)=0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1u)
        return (seq_t)0;
    seq_t p1 = pow2(n - 2);       /* 2^(n-2) */
    seq_t p2 = pow2(n - 1);       /* 2^(n-1) */
    if (n & 1u)                   /* subtract 1 if n is odd */
        --p2;
    return p1 * p2;
}
