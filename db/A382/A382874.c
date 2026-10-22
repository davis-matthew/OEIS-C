/*
 * A382874  Expansion of g.f. 2-hypergeom([3/2,7/2],[-1/2],4*x).
 * https://oeis.org/A382874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) =
 *            (2*n-1)*(2*n+1)^2*(2*n+3)*(2*n+5)*binom(2*n,n)/15 for n>=1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    // compute binomial coefficient C(2n, n)
    seq_t c = binom(2*n, n);

    seq_t res = c;
    res *= (seq_t)(2*n - 1);

    seq_t t = (seq_t)(2*n + 1);
    res *= t;          // (2n+1)
    res *= t;          // (2n+1)^2

    res *= (seq_t)(2*n + 3);
    res *= (seq_t)(2*n + 5);

    res /= (seq_t)15;

    return res;
}
