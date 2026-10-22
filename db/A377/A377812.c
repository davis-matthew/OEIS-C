/*
 * A377812  Number of quadruples of positive integers (x,y,a,b) such that a <
 *          b, gcd(a,b) = gcd(x,y) = 1 and a*x + b*y = n.
 * https://oeis.org/A377812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (x,y,a,b) of positive integers such
 *            that a < b, gcd(a,b) = gcd(x,y) = 1 and a*x + b*y = n
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n < 3) return 0;  /* for n = 1,2 the value is 0 */

    /* iterate over the smaller part size a = s */
    for (idx_t s = 1; s <= n - 2; ++s)
    {
        /* b = t must satisfy s < t <= n - s (otherwise a + b > n) */
        idx_t t_max = n - s;
        for (idx_t t = s + 1; t <= t_max; ++t)
        {
            /* require a and b to be coprime */
            if (gcd((seq_t)s, (seq_t)t) != 1) continue;

            /* maximum possible multiplicity for y (m2) so that t*m2 < n */
            idx_t max_m2 = (n - 1) / t;
            for (idx_t m2 = 1; m2 <= max_m2; ++m2)
            {
                idx_t prod = t * m2;          /* t * y */
                idx_t tmp  = n - prod;        /* a * x  (must be > 0) */

                if (tmp % s != 0) continue;   /* x is not an integer */

                idx_t m1 = tmp / s;           /* multiplicity for a (x) */

                /* ensure multiplicities are coprime */
                if (gcd((seq_t)m1, (seq_t)m2) == 1)
                    ++count;
            }
        }
    }

    return count;
}
