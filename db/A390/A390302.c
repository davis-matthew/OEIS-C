/*
 * A390302  E.g.f. A(x) satisfies A(x) = exp( x/(1-x^3) * A(x)^2 ).
 * https://oeis.org/A390302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (2*(n-3k)+1)^(n-3k-1) *
 *            binomial(n-2k-1,k) / (n-3k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Compute a(n) for OEIS sequence A390302 */
seq_t a(idx_t n)
{
    if (n == 0)   /* base case */
        return (seq_t)1;

    idx_t max_k = n / 3;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t m = n - 3 * k;                       /* m = n-3k */

        /* Compute n! / (m)! */
        seq_t fact_ratio = (seq_t)1;
        for (idx_t j = m + 1; j <= n; ++j)
            fact_ratio *= (seq_t)j;

        /* Compute (2*m+1)^(m-1); for m==0 the power is 1 */
        seq_t power = (seq_t)1;
        if (m > 0)
            power = big_pow((seq_t)(2 * m + 1), (seq_t)(m - 1));

        /* Binomial coefficient binomial(n-2k-1, k) */
        seq_t bin = binomial(n - 2 * k - 1, k);

        /* term = fact_ratio * bin * power */
        seq_t term = fact_ratio;
        term *= bin;
        term *= power;

        sum += term;
    }

    return sum;
}
