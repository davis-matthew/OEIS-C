/*
 * A380425  E.g.f. A(x) satisfies A(x) = exp( 2 * x * A(x)^(1/2) * exp(x *
 *          A(x)^(1/2)) ).
 * https://oeis.org/A380425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=1..n} k^(n-k) * (n+2)^(k-1) * binomial(n,k) with
 *            a(0) = 1. This implements the OEIS A380425 definition
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    /* Sum over k = 1 .. n (k = 0 gives 0 unless n = 0, which is handled above) */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(n,k) */
        seq_t bin = binomial(n, k);

        /* k^(n-k)  (k and exponent are cast to seq_t) */
        seq_t pow1 = big_pow((seq_t)k, (seq_t)(n - k));

        /* (n+2)^(k-1) */
        seq_t pow2 = big_pow((seq_t)(n + 2), (seq_t)(k - 1));

        /* term = bin * pow1 * pow2 */
        seq_t term = bin * pow1;
        term = term * pow2;

        /* accumulate */
        sum += term;
    }

    /* multiply the final sum by 2 as per the formula */
    return sum * (seq_t)2;
}
