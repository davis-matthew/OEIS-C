/*
 * A338658  a(n) = Sum_{d|n} d * binomial(d+n/d-1, d).
 * https://oeis.org/A338658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A338658 for n >= 1 */
/* Uses the helper binomial(idx_t n, idx_t k) from utils */

/* Include the binomial helper */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, for completeness */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t m = n / d;

            /* term for divisor d */
            seq_t term = (seq_t)d * binomial(d + m - 1, d);
            sum += term;

            /* term for divisor m, if distinct from d */
            if (m != d)
            {
                term = (seq_t)m * binomial(m + d - 1, m);
                sum += term;
            }
        }
    }

    return sum;
}
