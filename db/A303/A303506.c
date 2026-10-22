/*
 * A303506  G.f.: Sum_{n>=1} (-1)^(n-1) * x^(n^2)/(1 - x^n)^n.
 * https://oeis.org/A303506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} binomial(n/d-1, d-1) * (-1)^(d-1)
 * Range:     i8 n <= 35, i16 n <= 139, i32 n <= 479, i64 n <= 1747,
 *            i128 n <= 6215, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* term for divisor d1 */
            seq_t term  = binomial(d2 - 1, d1 - 1);
            seq_t sign  = ((d1 - 1) & 1) ? -1 : 1;
            result += sign * term;

            if (d1 != d2)
            {
                /* term for complementary divisor d2 */
                term  = binomial(d1 - 1, d2 - 1);
                sign  = ((d2 - 1) & 1) ? -1 : 1;
                result += sign * term;
            }
        }
    }
    return result;
}
