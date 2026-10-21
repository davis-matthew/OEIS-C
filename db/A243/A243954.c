/*
 * A243954  E.g.f.: A(x) = x/Series_Reversion(x*G(x)) where G(x) = Sum_{n>=0}
 *          n^n*x^n/n!.
 * https://oeis.org/A243954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0 a(n) = n! * Σ_{k=1}^{n-1} (-1)^{n-1-k} * n^{k-1}
 *            * binomial(n-2, n-1-k) / k! (Term k=0 contributes 0 because
 *            binomial(n-2,n-1)=0 for n>1) The function computes this
 *            directly; it is O(n)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    seq_t ff   = factorial(n);              // ff = n! / k! for current k
    seq_t pow  = (seq_t)1;                  // n^(k-1)
    idx_t m    = n - 2;                     // upper index for binomial
    idx_t r    = n - 2;                     // current lower index for binomial
    seq_t choose = (seq_t)1;                // binomial(m, r) initially binom(n-2,n-2)=1

    for (idx_t k = 1; k <= n - 1; ++k)
    {
        /* term = (n!/k!) * n^(k-1) * binomial(n-2, n-1-k) */
        seq_t term = ff * pow * choose;

        /* apply sign (-1)^(n-1-k) */
        if ( ((n - 1 - k) & 1) )
            term = -term;

        sum += term;

        /* update for next k only if we need it */
        if (k < n - 1)
        {
            ff   = ff / (seq_t)(k + 1);     // n! / (k+1)!
            pow  = pow * (seq_t)n;          // multiply by n for n^(k)
            /* update binomial: choose_{k+1} = choose_k * r / (m - r + 1) */
            seq_t numerator   = (seq_t)r;
            seq_t denominator = (seq_t)(m - r + 1);
            choose = (choose * numerator) / denominator;
            r--;                            // move to next lower index
        }
    }

    return sum;
}
