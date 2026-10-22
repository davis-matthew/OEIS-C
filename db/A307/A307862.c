/*
 * A307862  Coefficient of x^n in (1 + x - n*x^2)^n.
 * https://oeis.org/A307862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (-n)^k * binomial(n,k) * binomial(n-k,k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 12, i64 n <= 20, i128 n <= 37,
 *            big any n
 * Helpers:   binomial_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_nk.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t power = 1;                       /* (-n)^k, starting with k = 0 */
    seq_t neg_n = -(seq_t)n;               /* negative of n as seq_t */

    idx_t k_max = n / 2;                   /* floor(n/2) */

    for (idx_t k = 0; k <= k_max; ++k) {
        seq_t term = power;
        term = term * binomial_nk(n, k);
        term = term * binomial_nk(n - k, k);
        sum += term;

        power = power * neg_n;             /* prepare power for next k */
    }

    return sum;
}
