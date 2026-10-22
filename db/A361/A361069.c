/*
 * A361069  E.g.f. satisfies A(x) = exp( x/((1-x) * A(x)^3) ).
 * https://oeis.org/A361069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n} (-3*k+1)^(k-1) * binomial(n-1,n-k)/k!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (the term k=0 is zero because binomial(n-1,n)=0 for n>=1) */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* compute n! */
    seq_t nFact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        nFact *= (seq_t)i;

    seq_t coeff = nFact;   /* coeff = n! / k!  (initially k=1) */
    seq_t sum   = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* base = (-3*k + 1) */
        seq_t base = (seq_t)(-3 * (long long)k + 1);

        /* exponent = k-1 */
        seq_t basePow = big_pow(base, (seq_t)(k - 1));

        /* binomial(n-1, k-1) */
        seq_t bin = binomial(n - 1, k - 1);

        /* term = coeff * bin * basePow */
        seq_t term = coeff;
        term *= bin;
        term *= basePow;

        sum += term;

        /* prepare coeff for next k (k+1) */
        coeff = coeff / (seq_t)(k + 1);
    }

    return sum;
}
