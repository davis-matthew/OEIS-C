/*
 * A369267  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 / (1+x^3)^2 ).
 * https://oeis.org/A369267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/3⌋} binomial(2*n+2, k) *
 *            binomial(3*n-3*k+1, n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;                     /* floor(n/3) */
    seq_t n_plus_one = (seq_t)n + 1;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(2 * n + 2, k);            /* binomial(2*n+2, k)       */
        seq_t term2 = binomial(3 * n - 3 * k + 1, n - 3 * k); /* binomial(3*n-3k+1, n-3k) */
        sum += term1 * term2;
    }

    return sum / n_plus_one;
}
