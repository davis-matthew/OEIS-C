/*
 * A332051  Number of compositions of 2n where the multiplicity of the first
 *          part equals n.
 * https://oeis.org/A332051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{t=1}^{⌊n/2⌋} C(t+n-1, n-1) * C(n-t-1, t-1) (C(k,
 *            r) is binomial(k, r)). This computes the number of compositions
 *            of 2n in which the first part occurs exactly n times
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;     /* empty composition */

    seq_t result = (seq_t)1;   /* the composition consisting of n copies of 2 */

    idx_t max_t = n / (idx_t)2;
    for (idx_t t = (idx_t)1; t <= max_t; ++t) {
        /* C(t + n - 1, n - 1) */
        seq_t c1 = binomial(t + n - (idx_t)1, n - (idx_t)1);
        /* C(n - t - 1, t - 1) */
        seq_t c2 = binomial(n - t - (idx_t)1, t - (idx_t)1);
        result += c1 * c2;
    }
    return result;
}
