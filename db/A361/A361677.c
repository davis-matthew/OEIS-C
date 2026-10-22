/*
 * A361677  Constant term in the expansion of (1 + x + y + z + 1/(x*y) +
 *          1/(y*z) + 1/(z*x))^n.
 * https://oeis.org/A361677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * Sum_{k=0..⌊n/3⌋} binomial(3k,k)^2 * binomial(2k,k) *
 *            binomial(n,3k) * (OEIS A361677)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c3kk = binomial(3 * k, k);          /* binomial(3k, k) */
        seq_t c2kk = binomial(2 * k, k);          /* binomial(2k, k) */
        seq_t cn3k = binomial(n, 3 * k);          /* binomial(n, 3k) */

        seq_t term = c3kk;
        term = term * c3kk;   /* square */
        term = term * c2kk;
        term = term * cn3k;

        sum = sum + term;
    }
    return sum;
}
