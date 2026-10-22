/*
 * A377528  E.g.f. satisfies A(x) = 1/(1 - x * exp(x) * A(x))^4.
 * https://oeis.org/A377528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} k^(n-k) * binomial(5*k+3,k) /
 *            ((k+1)*(n-k)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the same sum directly, using the helpers big_pow and binomial. */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Compute the n-th term of OEIS A377528. */
seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* binomial(5*k+3, k) divided by (k+1) */
        seq_t binom = binomial((idx_t)(5 * k + 3), k);
        binom = binom / (seq_t)(k + 1);

        /* falling factorial n!/(n-k)! = n*(n-1)*...*(n-k+1) */
        seq_t falling = 1;
        for (idx_t j = 1; j <= k; ++j)
            falling *= (seq_t)(n - (j - 1));

        /* k^(n-k) */
        seq_t power = (k == 0 && n == k) ? 1 : big_pow((seq_t)k, (seq_t)(n - k));

        /* term = binom * falling * power */
        seq_t term = binom * falling;
        term *= power;

        result += term;
    }

    return result;
}
