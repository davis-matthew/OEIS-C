/*
 * A399334  The binomial matrix of the middle binomial coefficients, square
 *          array read by ascending antidiagonals.
 * https://oeis.org/A399334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = n-th term in the antidiagonal reading of the binomial
 *            matrix A(i,j) where A(i,i) = binomial(i, floor(i/2)) A(i,j) =
 *            Σ_{m=0}^{|i-j|} binomial(|i-j|, m) * s^{(|i-j|-m)} *
 *            binomial(min(i,j)+m, floor((min(i,j)+m)/2)) with s = 1 if j <=
 *            i, otherwise s = -1. Example: a(0)=1, a(1)=2, a(2)=0, a(3)=5,
 * Range:     i8 n <= 20, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the diagonal d containing the n-th element.
       The cumulative number of elements up to diagonal d-1 is
       d*(d+1)/2. Find the smallest d such that (d+1)*(d+2)/2 > n. */
    idx_t d = 0;
    while ( (d + 1) * (d + 2) / 2 <= n )
        ++d;

    /* Index within the diagonal */
    idx_t prev = d * (d + 1) / 2;
    idx_t offset = n - prev;

    /* The ordering inside each diagonal is from the largest row index
       down to the smallest: i = d - offset, j = offset. */
    idx_t i = d - offset;
    idx_t j = offset;

    /* Compute the matrix entry A(i,j) */
    if (i == j)
        return binom(i, i / 2);

    idx_t d_ref = (i > j) ? (i - j) : (j - i);
    idx_t p      = (i < j) ? i : j;
    int  s       = (j <= i) ? 1 : -1;

    seq_t sum = 0;
    for (idx_t m = 0; m <= d_ref; ++m)
    {
        seq_t term = binom(d_ref, m);

        /* Multiply by s^(d_ref-m).  When s == -1 this is just a sign. */
        if (s == -1 && ((d_ref - m) & 1))
            term = -term;

        /* Multiply by the middle binomial coefficient M(p+m). */
        term *= binom(p + m, (p + m) / 2);

        sum += term;
    }
    return sum;
}
