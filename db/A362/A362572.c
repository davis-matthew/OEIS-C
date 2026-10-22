/*
 * A362572  E.g.f. satisfies A(x) = exp(x * A(x)^(x/2)).
 * https://oeis.org/A362572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} ((n-k)/2)^k *
 *            binomial(n-k-1,k)/(n-k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Compute contribution for this k */
        seq_t term = 1;
        idx_t rem = k;            /* remaining power of 2 to cancel */

        /* Multiply by falling factorial n!/(n-k)! */
        for (idx_t i = 1; i <= k; ++i) {
            term *= (seq_t)(n - i + 1);
            while (rem > 0 && (term & (seq_t)1) == 0) {
                term >>= 1;
                --rem;
            }
        }

        /* Multiply by binomial coefficient */
        term *= binomial((idx_t)(n - k - 1), k);
        while (rem > 0 && (term & (seq_t)1) == 0) {
            term >>= 1;
            --rem;
        }

        /* Multiply by (n-k)^k, cancelling 2's as we go */
        seq_t base = (seq_t)(n - k);
        for (idx_t i = 0; i < k; ++i) {
            term *= base;
            while (rem > 0 && (term & (seq_t)1) == 0) {
                term >>= 1;
                --rem;
            }
        }

        /* At this point rem should be zero; term holds the k-th summand */
        sum += term;
    }

    return sum;
}
