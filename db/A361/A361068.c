/*
 * A361068  E.g.f. satisfies A(x) = exp( x/((1-x) * A(x)^2) ).
 * https://oeis.org/A361068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A361068
   Formula: a(n) = n! * Sum_{k=1..n} (-2*k+1)^(k-1) * binomial(n-1, k-1) / k! */
/* (The term for n = 0 is defined to be 1.) */

#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Computes the n-th term of A361068 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t fact_n = factorial(n);

    for (idx_t k = 1; k <= n; ++k)
    {
        /* base = -2*k + 1 (as seq_t) */
        seq_t base = (seq_t)(-(2 * (seq_t)k) + 1);

        /* (base)^(k-1) */
        seq_t powv = big_pow(base, k - 1);

        /* binomial(n-1, k-1) */
        seq_t bin = binomial(n - 1, k - 1);

        /* fact_n / k! (exact integer division) */
        seq_t fact_k = factorial(k);
        seq_t factor = fact_n / fact_k;

        /* term = factor * powv * bin */
        seq_t term = factor * powv;
        term = term * bin;

        sum += term;
    }

    return sum;
}
