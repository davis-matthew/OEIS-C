/*
 * A372950  a(n) = Sum_{1 <= x_1, x_2 <= n} ( n/gcd(x_1, x_2, n) )^3.
 * https://oeis.org/A372950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * (n/d)^3 * sigma_5(d)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 86, u64 n <= 7140,
 *            u128 n <= 10000, big any n
 * Helpers:   mobius, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"   /* seq_t mobius(idx_t m); */
#include "../utils/sigma_k.h"  /* seq_t sigma_k(idx_t m, uint8_t k); */

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;       /* second divisor */

            /* first divisor: d1 */
            int mu1 = (int)mobius(n / d1);   /* mu(n/d1) = mu(d2) */
            if (mu1 != 0)
            {
                seq_t nd1 = (seq_t)(n / d1);          /* n/d1 */
                seq_t term = nd1 * nd1 * nd1;          /* (n/d1)^3 */
                term *= sigma_k(d1, 5);               /* * sigma_5(d1) */

                if (mu1 == 1)
                    result += term;
                else                /* mu1 == -1 */
                    result -= term;
            }

            /* second divisor: d2, only if distinct */
            if (d1 != d2)
            {
                int mu2 = (int)mobius(n / d2);   /* mu(n/d2) = mu(d1) */
                if (mu2 != 0)
                {
                    seq_t nd2 = (seq_t)(n / d2);          /* n/d2 */
                    seq_t term = nd2 * nd2 * nd2;          /* (n/d2)^3 */
                    term *= sigma_k(d2, 5);               /* * sigma_5(d2) */

                    if (mu2 == 1)
                        result += term;
                    else                /* mu2 == -1 */
                        result -= term;
                }
            }
        }
    }

    return result;
}
