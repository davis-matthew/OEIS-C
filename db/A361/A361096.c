/*
 * A361096  E.g.f. A(x) satisfies A(x) = exp( 1/(1 - x/A(x)^2) - 1 ).
 * https://oeis.org/A361096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 For n >= 1, a(n) = n! * Σ_{k=1..n} (-2n+1)^{k-1} *
 *            binom(n-1, k-1) / k! This can be computed incrementally using
 *            integer arithmetic only
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* n!  */
    seq_t prod = factorial(n);

    /* r = -(2n-1)  */
    seq_t r = -( (seq_t)2 * (seq_t)n - (seq_t)1 );

    /* running power r^{k-1}  */
    seq_t pow = (seq_t)1;

    /* binom(n-1, k-1)  */
    seq_t binom = (seq_t)1;

    /* sum of terms  */
    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* term = prod * pow * binom  */
        seq_t term = prod * pow * binom;
        sum += term;

        if (k == n)
            break;

        /* update for next k */
        pow *= r;
        binom = binom * (seq_t)(n - k) / (seq_t)k;
        prod /= (seq_t)(k + 1);
    }

    return sum;
}
