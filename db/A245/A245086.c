/*
 * A245086  Central values of the n-th discrete Chebyshev polynomials of order
 *          2n.
 * https://oeis.org/A245086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n is odd; otherwise a(2m) = (-1)^m * (3m)!/(m!^3) =
 *            (-1)^m * binomial(3m,m) * binomial(2m,m)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 15, i64 n <= 29, i128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n & 1U)          /* odd indices give zero */
        return 0;

    idx_t m = n >> 1;      /* m = n/2 */
    seq_t c1 = binomial(3 * m, m);   /* (3m)!/(m!(2m)!)   */
    seq_t c2 = binomial(2 * m, m);   /* (2m)!/(m!^2)      */
    seq_t res = c1 * c2;             /* (3m)!/(m!^3)      */
    if (m & 1U)               /* apply (-1)^m   */
        res = -res;
    return res;
}
