/*
 * A364507  a(n) = (5*n)!*(4*n)! / ((3*n)!^2 * (2*n)! * n!).
 * https://oeis.org/A364507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, n) * binomial(5*n, 2*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t t1 = binomial(4*n, n);   /* C(4n, n) */
    seq_t t2 = binomial(5*n, 2*n); /* C(5n, 2n) */
    return t1 * t2;
}
