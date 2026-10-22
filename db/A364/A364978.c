/*
 * A364978  E.g.f. satisfies A(x) = 1 + x*exp(x*A(x)^2).
 * https://oeis.org/A364978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(2n-2k+1,k) /
 *            ((2n-2k+1)*(n-k)!) Simplification used: a(n) = Σ_{k=0}^n (n)_k *
 *            k^(n-k) * binomial(2n-2k+1,k) / (2n-2k+1) where (n)_k =
 *            n*(n-1)*…*(n-k+1) is the falling factorial
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    seq_t sum = 0;
    seq_t falling = 1;                /* (n)_0 = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* k^(n-k) */
        seq_t powk = big_pow((seq_t)k, (seq_t)(n - k));

        /* binomial(2n-2k+1, k) */
        seq_t bin = binomial(2 * n - 2 * k + 1, k);

        /* term = (n)_k * k^(n-k) * binomial / (2n-2k+1) */
        seq_t term = falling * powk * bin;
        term /= (seq_t)(2 * n - 2 * k + 1);

        sum += term;

        /* update falling for next k: (n)_{k+1} = (n)_k * (n-k) */
        if (k < n)                    /* avoid multiplying by 0 after the last iteration */
            falling *= (seq_t)(n - k);
    }

    return sum;
}
