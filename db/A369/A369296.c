/*
 * A369296  Expansion of (1/x) * Series_Reversion( x * (1-x) * (1-x^3)^2 ).
 * https://oeis.org/A369296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binom(2n + k + 1, k) *
 *            binom(2n - 3k, n - 3k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;                         /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t top    = binomial(2 * n + k + 1, k);
        seq_t bottom = binomial(2 * n - 3 * k, n - 3 * k);
        sum += top * bottom;
    }
    return sum / ((seq_t)n + 1);
}
