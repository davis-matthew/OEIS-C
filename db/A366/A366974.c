/*
 * A366974  Expansion of Sum_{k >=1} x^(2*k)/(1-x^k)^(k+1).
 * https://oeis.org/A366974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(d + n/d - 2, d)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety for indices < 1 */
    if (n == 1) return 0;         /* a(1) = 0 */

    seq_t result = 0;

    /* enumerate divisors in pairs (d, n/d) */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;

        idx_t d1 = d;
        idx_t nd1 = n / d1;
        idx_t a1 = d1 + nd1 - 2;
        if (a1 >= d1)                 /* binomial defined only for k ≤ n */
            result += binomial(a1, d1);

        if (d1 != nd1)
        {
            idx_t d2 = nd1;
            idx_t nd2 = d1;
            idx_t a2 = d2 + nd2 - 2;
            if (a2 >= d2)
                result += binomial(a2, d2);
        }
    }

    return result;
}
