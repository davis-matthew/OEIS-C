/*
 * A362482  E.g.f. satisfies A(x) = exp(x - x^4 * A(x)^4).
 * https://oeis.org/A362482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/4⌋} (-1)^k * (4*k+1)^(n-3*k-1) / (k! *
 *            (n-4*k)!)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the terms directly from the formula. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Upper bound for k in the sum */
    idx_t limit = n / 4;

    seq_t result = 0;
    seq_t falling = 1;   /* falling factorial: n!/(n-4*k)! */
    seq_t fact_k = 1;    /* k! */

    for (idx_t k = 0; k <= limit; ++k)
    {
        /* Exponent for (4k+1)^(n-3k-1) */
        idx_t exp = n - 3 * k - 1;

        /* Base of the power */
        seq_t base = (seq_t)(4 * k + 1);

        /* Compute the power term */
        seq_t pow_term = big_pow(base, (seq_t)exp);

        /* Term for the current k:
           falling / fact_k * pow_term  (sign handled later) */
        seq_t term = pow_term * falling;
        term /= fact_k;

        /* Alternating sign */
        if (k & 1)
            result -= term;
        else
            result += term;

        /* Prepare cumulative products for the next iteration */
        if (k != limit)
        {
            seq_t m1 = (seq_t)(n - 4 * k);
            seq_t m2 = (seq_t)(n - 4 * k - 1);
            seq_t m3 = (seq_t)(n - 4 * k - 2);
            seq_t m4 = (seq_t)(n - 4 * k - 3);

            falling = falling * m1 * m2 * m3 * m4;
            fact_k *= (k + 1);
        }
    }

    return result;
}
