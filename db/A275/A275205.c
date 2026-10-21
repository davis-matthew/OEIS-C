/*
 * A275205  Partial sums of the Dirichlet inverse of the Euler totient
 *          function.
 * https://oeis.org/A275205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k = 1..n} k * mu(k) * floor(n / k)
 * Range:     i8 n <= 48, i16 n <= 2754, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 0, a(3) = -2, ...    See OEIS A275205 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t mu = mobius(k);
        if (mu != 0)
        {
            /* floor(n/k) is simply integer division */
            seq_t term = (seq_t)k * mu * (seq_t)(n / k);
            total += term;
        }
    }
    return total;
}
