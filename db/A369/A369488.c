/*
 * A369488  Expansion of (1/x) * Series_Reversion( x / (1-x)^2 * (1-x-x^2)^3
 *          ).
 * https://oeis.org/A369488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Σ_{k=0..⌊n/2⌋} binomial(3*n + k + 2, k) *
 *            binomial(2*n - k, n - 2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>               /* For uint64_t used by idx_t in the build */
#include "../utils/binom.h"       /* Provides: seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Compute the summation for k = 0 .. floor(n/2) */
    idx_t maxk = n / 2;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* binomial(3*n + k + 2, k) */
        seq_t t1 = binom(3 * n + k + 2, k);

        /* binomial(2*n - k, n - 2*k) */
        seq_t t2 = binom(2 * n - k, n - 2 * k);

        /* Accumulate the product */
        sum += t1 * t2;
    }

    /* Divide by (n+1) to obtain a(n) */
    return sum / ((seq_t)n + 1);
}
