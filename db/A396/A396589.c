/*
 * A396589  Expansion of e.g.f. F_2(x)/x and F_k(x) is the k-th iteration of
 *          x*G(x) with G(x) = exp(x*G(x)^2).
 * https://oeis.org/A396589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * (2*k+1)^(k-1) * (2*n-k+1)^(n-k-1) *
 *            binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Computes sequence A396589. */
seq_t a(idx_t n)
{
    if (n == 0)      /* a(0) = 1 by definition */
        return (seq_t)1;

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(n, k);

        if (k == n) {
            /* k = n : term = binomial * (2k+1)^(k-1) ;  (k+1) cancels with (2n-k+1)^(-1) */
            if (k > 0) {
                seq_t pow1 = big_pow((seq_t)(2 * k + 1), (seq_t)(k - 1));
                term *= pow1;
            }
            /* For k==0 (which implies n==0) already handled above */
        } else {
            /* Normal case */
            term *= (seq_t)(k + 1);

            if (k > 0) {
                seq_t pow1 = big_pow((seq_t)(2 * k + 1), (seq_t)(k - 1));
                term *= pow1;
            }

            seq_t pow2 = big_pow((seq_t)(2 * n - k + 1), (seq_t)(n - k - 1));
            term *= pow2;
        }

        sum += term;
    }

    return sum;
}
