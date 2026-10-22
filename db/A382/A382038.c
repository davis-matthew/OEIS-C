/*
 * A382038  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * B(x)^4)
 *          ), where B(x) = 1 + x*B(x)^4 is the g.f. of A002293.
 * https://oeis.org/A382038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Σ_{k=0}^{n-1} (n+1)^(n-k-1) * binomial(4n,k) /
 *            (n-k-1)! The implementation rewrites the sum as a(n) =
 *            Σ_{k=0}^{n-1} ( (n-1)! / (n-k-1)! ) * (n+1)^(n-k-1) *
 *            binomial(4n,k) so that every summand is an integer. This avoids
 *            fractional intermediate values and works with arbitrary‐preci...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* offset 0 */
    if (n == 0)
        return (seq_t)1;

    /* base for powers (n+1) */
    seq_t base = (seq_t)(n + 1);

    /* (n+1)^(n-1) starts the first term of the sum */
    seq_t pow_val = seq_pow(base, n - 1);

    /* factorial ratio (n-1)! / m!  starting with m = n-1 (empty product = 1) */
    seq_t fac_ratio = (seq_t)1;

    /* current binomial coefficient binomial(4n, k) */
    seq_t binom = (seq_t)1;

    /* running sum of the integer summands */
    seq_t sum = (seq_t)0;

    /* current m = n-k-1, initially m = n-1 */
    idx_t m = n - 1;

    for (idx_t k = 0; k < n; ++k)
    {
        /* add this integer summand to the sum */
        sum += fac_ratio * pow_val * binom;

        /* prepare for the next k, unless we are finished */
        if (k + 1 < n)
        {
            /* update factorial ratio: multiply by the new lower bound m */
            fac_ratio *= (seq_t)m;

            /* next power: divide by base (exact) */
            pow_val = pow_val / base;

            /* next binomial: binom_{k+1} = binom_k * (4n - k) / (k+1) */
            binom = (binom * (seq_t)(4 * n - k)) / (seq_t)(k + 1);

            /* decrement m for the next iteration */
            --m;
        }
    }

    return sum;
}
