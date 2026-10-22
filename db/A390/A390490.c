/*
 * A390490  Dirichlet g.f.: (zeta(2*s))^2 / (zeta(s) * zeta(4*s)).
 * https://oeis.org/A390490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390490: multiplicative function
   a(p)        = -1
   a(p^e)      = 2*(-1)^e   for e > 1
   a(1)        = 1
   (OEIS formula: Dirichlet g.f. (zeta(2s))^2/(zeta(s)*zeta(4s)) ) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            if (e == 1)
                result *= (seq_t)(-1);
            else
            {
                seq_t mult = 2;
                if (e % 2 == 1)   /* odd exponent -> (-1)^e = -1 */
                    mult = -2;
                result *= mult;
            }
        }
    }

    if (m > 1) /* remaining prime factor */
        result *= (seq_t)(-1);

    return result;
}
