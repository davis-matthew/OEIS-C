/*
 * A370617  Coefficient of x^n in the expansion of 1 / (1-x-x^2)^(2*n).
 * https://oeis.org/A370617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(2*n + k - 1, k) * binomial(3*n -
 *            k - 1, n - 2*k) (OEIS A370617: coefficient of x^n in
 *            1/(1-x-x^2)^(2*n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1, avoid negative binomial arguments */
    if (n == 0)
        return (seq_t)1;

    idx_t maxk = n >> 1;  /* floor(n/2) */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* first binomial: binom(2*n + k - 1, k) */
        seq_t b1 = binomial(2 * n + k - 1, k);

        /* second binomial: binom(3*n - k - 1, n - 2*k) */
        seq_t b2 = binomial(3 * n - k - 1, n - 2 * k);

        /* term_k = b1 * b2 */
        seq_t term = b1 * b2;

        sum += term;
    }

    return sum;
}
