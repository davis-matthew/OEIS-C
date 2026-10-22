/*
 * A380721  E.g.f. A(x) satisfies A(x) = exp(x / (1 - x*A(x))^3) / (1 -
 *          x*A(x))^2.
 * https://oeis.org/A380721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n-k+1)^(k-1) * binomial(3*n+1, n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
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
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* n! */
    seq_t fact_n = factorial(n);

    /* term for k = 0 : (n! * binomial(3n+1, n)) / (n+1) */
    seq_t bin0 = binomial(3 * n + 1, n);
    seq_t sum = (fact_n * bin0) / (n + 1);

    /* factor n! / k! for k >= 1 */
    seq_t fact_ratio = fact_n;   /* k = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* (n - k + 1)^(k-1) */
        seq_t base   = n - k + 1;
        seq_t power  = big_pow(base, k - 1);

        /* binomial(3n+1, n-k) */
        seq_t bin = binomial(3 * n + 1, n - k);

        /* term contribution : power * bin * fact_ratio = n! * (n-k+1)^(k-1) * bin / k! */
        seq_t term = power * bin * fact_ratio;
        sum += term;

        /* prepare fact_ratio for next k: divide by (k+1) to get n! / (k+1)! */
        if (k < n)
            fact_ratio /= (k + 1);
    }

    return sum;
}
