/*
 * A307428  Dirichlet g.f.: zeta(2*s) / (zeta(s) * zeta(3*s)).
 * https://oeis.org/A307428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(p) = -1, a(p^2)=1, a(p^e)=2*(-1)^e
 *            for e>=3
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            seq_t val;
            if (e == 1)
                val = -1;
            else if (e == 2)
                val = 1;
            else
                val = (e & 1) ? (seq_t)(-2) : (seq_t)2; /* 2*(-1)^e */
            result *= val;
        }
    }
    /* Remaining prime factor > sqrt(original n) */
    if (temp > 1)
    {
        /* e = 1 */
        result *= (seq_t)(-1);
    }
    return result;
}
