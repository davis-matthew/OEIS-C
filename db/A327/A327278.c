/*
 * A327278  a(n) = Sum_{d|n, d odd} d * mu(d) * mu(n/d).
 * https://oeis.org/A327278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy, changed
 * Formula:   a(n) = Sum_{d|n, d odd} d * mu(d) * mu(n/d)
 * Range:     i8 n <= 92, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i != 0)
            continue;

        /* divisor i */
        if ((i & 1) != 0)      /* i is odd */
        {
            int mu_i  = mu_of(i);
            int mu_j  = mu_of(n / i);
            sum += (seq_t)i * (seq_t)mu_i * (seq_t)mu_j;
        }

        /* complementary divisor j = n / i */
        idx_t j = n / i;
        if (j != i && (j & 1) != 0)   /* j is odd and distinct from i */
        {
            int mu_j  = mu_of(j);
            int mu_i  = mu_of(i);
            sum += (seq_t)j * (seq_t)mu_j * (seq_t)mu_i;
        }
    }
    return sum;
}
