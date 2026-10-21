/*
 * A244468  E.g.f.: Sum_{n>=0} Series_Reversion( x/exp(n*x) )^n / n!.
 * https://oeis.org/A244468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n-1, k-1) * (n*k)^(n-k) , a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions are provided by the build.
   We use binomial(a, b) to compute binomial coefficients
   and big_pow(base, exp) to compute integer powers. */
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;
    /* Iterate over k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        /* binomial coefficient binom(n-1, k-1) */
        seq_t bin = binomial(n - 1, k - 1);

        /* base = n * k */
        seq_t base = (seq_t)n * (seq_t)k;

        /* exponent = n - k */
        seq_t exp = (seq_t)(n - k);

        /* (n*k)^(n-k) */
        seq_t pow_val = big_pow(base, exp);

        /* term = binomial * pow_val */
        seq_t term = bin * pow_val;

        /* add to the accumulator */
        sum += term;
    }
    return sum;
}
