/*
 * A340143  Möbius transform of A160595, where A160595(x) = phi(x)/gcd(phi(x),
 *          x-1).
 * https://oeis.org/A340143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * phi(d)/gcd(phi(d), d-1)
 * Range:     i8 n <= 260, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   gcd, mobius, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/mobius.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* first divisor d = i */
            idx_t    d  = i;
            idx_t    co = n / d;
            seq_t    mu = mobius(co);
            if (mu != 0)
            {
                seq_t phi_d = phi(d);
                seq_t g = phi_d / gcd(phi_d, (seq_t)(d - 1));
                sum += mu * g;
            }

            /* partner divisor d = n / i, different from i */
            if (i != n / i)
            {
                d  = n / i;
                co = n / d;   /* equals i */
                mu = mobius(co);
                if (mu != 0)
                {
                    seq_t phi_d = phi(d);
                    seq_t g = phi_d / gcd(phi_d, (seq_t)(d - 1));
                    sum += mu * g;
                }
            }
        }
    }
    return sum;
}
