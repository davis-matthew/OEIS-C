/*
 * A375290  Expansion of 1/((1 - x + x^4)^2 + 4*x^5).
 * https://oeis.org/A375290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/2) * Σ_{k=0..⌊n/4⌋} (-1)^k * binomial(2*n-6*k+2,
 *            2*k+1)
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 30, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        idx_t upper = 2 * n - 6 * k + 2;
        idx_t lower = 2 * k + 1;
        seq_t term  = binom(upper, lower);
        if (k & 1)
            total -= term;          /* (-1)^k for odd k */
        else
            total += term;
    }
    return total / 2;                /* division by 2, exact for all n */
}
