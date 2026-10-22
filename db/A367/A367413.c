/*
 * A367413  Expansion of (1/x) * Series_Reversion( x * (1-x-x^3/(1-x)^2) ).
 * https://oeis.org/A367413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/3⌋} binomial(n+k, k) *
 *            binomial(2n, n-3k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binom(n + k, k);
        seq_t c2 = binom(2 * n, n - 3 * k);
        sum += c1 * c2;
    }
    return sum / ((seq_t)n + 1);
}
