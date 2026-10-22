/*
 * A380016  Expansion of e.g.f. 1/(exp(-3*x) - 3*x)^(1/3).
 * https://oeis.org/A380016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * P_k * (3k+1)^(n-k) where P_k =
 *            ∏_{i=0}^{k-1} (3i+1) (P_0 = 1). This follows from the
 *            binomial‑series expansion of 1/(exp(-3x)-3x)^(1/3) and
 *            simplifies the original definition without any rational
 *            arithmetic
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Compute a(n) for arbitrary-precision seq_t. */
seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* accumulator for the sum */
    seq_t binom = 1;          /* binomial(n,0) */
    seq_t prod  = 1;          /* P_0 = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* Base for the power term. */
        seq_t base = (seq_t)(3 * k + 1);
        /* Power (3k+1)^(n-k). */
        seq_t powv = seq_pow(base, n - k);

        /* Contribution of this k. */
        sum += binom * prod * powv;

        /* Prepare variables for next k, if any. */
        if (k < n) {
            /* P_{k+1} = P_k * (3k+1). */
            prod = prod * (seq_t)(3 * k + 1);
            /* binomial(n,k+1) = binomial(n,k) * (n-k) / (k+1). */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
