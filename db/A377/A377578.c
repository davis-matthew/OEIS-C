/*
 * A377578  E.g.f. satisfies A(x) = (1 + x * exp(x*A(x)))^3.
 * https://oeis.org/A377578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(3*n-3*k+3,k) /
 *            ((n-k+1)*(n-k)! )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    seq_t product = 1;        /* P_k for current k, starts with k=1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute k^(n-k) */
        seq_t k_pow = pow_uint((uint64_t)k, (idx_t)(n - k));

        /* binomial(3*n-3*k+3, k) */
        seq_t bin = binomial((idx_t)(3 * n - 3 * k + 3), k);

        /* term = k^(n-k) * bin * product */
        seq_t term = k_pow * bin;
        term = term * product;

        sum += term;

        /* update product for next k: P_{k+1} = P_k * (n-k+1) */
        product *= (seq_t)(n - k + 1);
    }

    return sum;
}
