/*
 * A331726  Expansion of e.g.f. -LambertW(-x/(1 - x)) / (1 - x).
 * https://oeis.org/A331726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * k! * (n - k)^(n - k - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t bin = binomial(n, k);            /* binomial(n,k) */
        seq_t term = bin * bin;                /* bin^2 */
        term *= factorial(k);                 /* * k! */
        seq_t base = (seq_t)(n - k);           /* n - k */
        seq_t exp  = (seq_t)(n - k - 1);       /* n - k - 1 */
        term *= big_pow(base, exp);            /* * (n - k)^(n - k - 1) */
        sum += term;
    }
    return sum;
}
