/*
 * A326415  Dirichlet g.f.: zeta(2*s) / zeta(s)^3.
 * https://oeis.org/A326415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} f(p^e), where f(p^e) = -3 if e = 1 4·(-1)^e
 *            otherwise (multiplicative function with a(p) = -3 and a(p^e) =
 *            4*(-1)^e for e>1)
 * Range:     i8 n <= 1259, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1;   /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* trial division is safe for the maximal index (≈10 000) */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }

            if (exp == 1)
                result *= (seq_t)-3;
            else
                result *= (seq_t)(4 * ((exp & 1) ? -1 : 1));
        }
    }

    /* remaining prime factor, if any */
    if (m > 1)
        result *= (seq_t)-3;

    return result;
}
