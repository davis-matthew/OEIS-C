/*
 * A377548  Expansion of e.g.f. (1/x) * Series_Reversion( x*(1 - x*exp(x))^3
 *          ).
 * https://oeis.org/A377548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * 3 * Σ_{k=0..n} * ( Π_{i=0}^{k-1} (n-i) ) * k^(n-k) *
 *            binomial(3*n + k + 3, k) *
 *            ----------------------------------------------------------------
 *            * 3*n + k + 3 * * (The product over an empty set is 1.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum  = 0;           /* accumulator for the whole sum */
    seq_t prod = 1;           /* Π_{i=0}^{k-1} (n-i)  for the current k */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* k^(n-k) */
        seq_t pow_val = big_pow((seq_t)k, (seq_t)(n - k));

        /* binomial(3*n + k + 3, k) */
        seq_t bin = binomial(3 * n + k + 3, k);

        /* numerator: 3 * prod * pow_val * bin */
        seq_t num = 3 * prod;
        num *= pow_val;
        num *= bin;

        /* denominator: 3*n + k + 3 */
        seq_t denom = (seq_t)(3 * n + k + 3);

        /* integer division – guaranteed to be exact */
        seq_t term = num / denom;

        sum += term;

        /* update prod for next k (product of (n-i) for i=0..k) */
        if (k < n)
            prod *= (seq_t)(n - k);
    }

    return sum;
}
