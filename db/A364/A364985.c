/*
 * A364985  E.g.f. satisfies A(x) = 1 + x*A(x)^3*exp(x*A(x)^2).
 * https://oeis.org/A364985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(2*n+k+1, k) /
 *            ((2*n+k+1)*(n-k)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) -> seq_t */
#include "../utils/factorial.h"  /* factorial(idx_t) -> seq_t */
#include "../utils/big_pow.h"    /* big_pow(seq_t, seq_t) -> seq_t */

seq_t a(idx_t n)
{
    /* k^(n-k) * binomial(2*n+k+1, k) * (n!/(n-k)!)/(2*n+k+1) summed over k */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute k^(n-k) */
        seq_t kpow;
        if (k == 0)
            kpow = (n == 0) ? 1 : 0;      /* 0^0 = 1, otherwise 0 */
        else
            kpow = big_pow((seq_t)k, (seq_t)(n - k));

        if (kpow == 0)               /* term will be zero for k=0 and n>0 */
            continue;

        /* binomial(2*n+k+1, k) */
        seq_t bin = binomial(2*n + k + 1, k);

        /* product (n!/(n-k)!) = n*(n-1)*...*(n-k+1) */
        seq_t prod = 1;
        for (idx_t t = 0; t < k; ++t)
            prod *= (seq_t)(n - t);

        /* numerator = kpow * bin * prod
         * denominator = (2*n + k + 1) */
        seq_t numerator = kpow * bin;
        numerator *= prod;

        seq_t term = numerator / (seq_t)(2*n + k + 1);
        sum += term;
    }

    return sum;
}
