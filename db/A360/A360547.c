/*
 * A360547  E.g.f. satisfies A(x) = exp( x * ( exp(x) * A(x) )^2 ).
 * https://oeis.org/A360547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*k)^(n-k) * (2*k+1)^(k-1) * binomial(n,k),
 *            * with the convention that 0^0 = 1 and (2*k+1)^(k-1) is omitted
 *            for k = 0 * (except for the trivial case n = 0, whose value is
 *            1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(n,k) */
        seq_t bin = binomial(n, k);

        /* (2*k)^(n-k) */
        seq_t pow1 = big_pow((seq_t)(2 * k), (seq_t)(n - k));

        /* (2*k+1)^(k-1) */
        seq_t pow2 = big_pow((seq_t)(2 * k + 1), (seq_t)(k - 1));

        /* multiply all components */
        seq_t term = bin;
        term *= pow1;
        term *= pow2;

        sum += term;
    }

    return sum;
}
