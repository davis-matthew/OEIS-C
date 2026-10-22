/*
 * A380719  E.g.f. A(x) satisfies A(x) = exp(x / (1 - x*A(x))^2) / (1 -
 *          x*A(x)).
 * https://oeis.org/A380719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (n-k+1)^(k-1) * binomial(2n, n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A380719 (offset 0). */

#include <stddef.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b);      */
#include "../utils/factorial.h"  /* seq_t factorial(idx_t n);             */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* factorial of n */
    seq_t nfact = factorial(n);

    /* running denominator for k! (k! for current k) */
    seq_t denom_fact = (seq_t)1;

    seq_t result = (seq_t)0;

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        seq_t bin = binomial((idx_t)(2 * n), (idx_t)(n - k));
        seq_t term;

        if (k == (idx_t)0)
        {
            /* term = (nfact * bin) / (n + 1) */
            seq_t denom = (seq_t)(n + 1);
            term = (nfact * bin) / denom;
        }
        else
        {
            /* term = (nfact / k!) * (n-k+1)^(k-1) * bin */
            seq_t mult = nfact / denom_fact;
            seq_t base = (seq_t)(n - k + 1);
            seq_t exp  = (seq_t)(k - 1);
            seq_t pow = big_pow(base, exp);
            term = mult * pow;
            term = term * bin;
        }

        result += term;

        /* update k! for next iteration: denom_fact *= (k+1) */
        denom_fact = denom_fact * (seq_t)(k + 1);
    }

    return result;
}
