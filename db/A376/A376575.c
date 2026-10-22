/*
 * A376575  E.g.f. A(x) satisfies A(x) = exp(x*A(x)/(1 - x^2)).
 * https://oeis.org/A376575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/2⌋} (n-2k+1)^(n-2k-1) * binomial(n-k-1,
 *            k) / (n-2k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum      = (seq_t)0;
    seq_t fact_n   = factorial(n);          /* n! */

    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - 2 * k;                /* m = n-2k */

        /* factorial ratio: n! / (n-2k)!  (exact division) */
        seq_t fact_div = fact_n / factorial(m);

        /* binomial term */
        seq_t bin = binomial(n - k - 1, k);

        /* (m+1)^(m-1), with the convention that the term is 1 when m==0 */
        seq_t pow_val = (m == 0) ? (seq_t)1 : big_pow((seq_t)(m + 1), (idx_t)(m - 1));

        /* term = (n!/(n-2k)!) * binomial * (m+1)^(m-1) */
        seq_t term = fact_div;
        term = term * bin;
        term = term * pow_val;

        sum += term;
    }

    return sum;
}
