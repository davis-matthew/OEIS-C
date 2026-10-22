/*
 * A369296  Expansion of (1/x) * Series_Reversion( x * (1-x) * (1-x^3)^2 ).
 * https://oeis.org/A369296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binom(2n + k + 1, k) *
 *            binom(2n - 3k, n - 3k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;                         /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t top    = binom(2 * n + k + 1, k);
        seq_t bottom = binom(2 * n - 3 * k, n - 3 * k);
        sum += top * bottom;
    }
    return sum / ((seq_t)n + 1);
}
