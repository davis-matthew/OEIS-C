/*
 * A369484  Expansion of (1/x) * Series_Reversion( x / ((1+x) * (1+x+x^3)^2)
 *          ).
 * https://oeis.org/A369484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binom(2n+2, k) * binom(3n-k+3,
 *            n-3k) for n ≥ 0 (OEIS offset 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;                 /* largest k in the sum     */
    for (idx_t k = 0; k <= max_k; ++k) {
        /* binomial(2n+2, k) */
        seq_t bin1 = binom(2 * n + 2, k);

        /* binomial(3n - k + 3, n - 3k) */
        idx_t m = 3 * n - k + 3;
        idx_t r = n - 3 * k;
        seq_t bin2 = binom(m, r);

        sum += bin1 * bin2;
    }
    /* division is exact by definition of the sequence */
    return sum / ((seq_t)(n + 1));
}
