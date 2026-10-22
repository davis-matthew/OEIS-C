/*
 * A337130  a(n) is the sum of all products of pairs of numbers joined by the
 *          diagonals of an n-gon when its vertices are numbered from 1 to n
 *          in order.
 * https://oeis.org/A337130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(n+1,4) - n for n>=3, a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 431, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute C(N,4) with safe intermediate operations */
static seq_t binom4(seq_t N)
{
    seq_t res = 1;
    for (idx_t i = 0; i < 4; ++i)
    {
        seq_t numerator   = N - (seq_t)i;
        seq_t denominator = (seq_t)(i + 1);

        /* Reduce numerator/denominator */
        seq_t g1 = gcd(numerator, denominator);
        numerator   /= g1;
        denominator /= g1;

        /* Reduce denominator from current result */
        seq_t g2 = gcd(res, denominator);
        res   /= g2;
        denominator /= g2;

        /* Now denominator is 1; multiply safely */
        res *= numerator;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    seq_t binom = binom4((seq_t)(n + 1));
    seq_t result = binom * (seq_t)3;
    result -= (seq_t)n;
    return result;
}
