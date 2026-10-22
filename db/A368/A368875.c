/*
 * A368875  a(n) = 24*(3*n + 1)!/(n!*((n + 2)!)^2).
 * https://oeis.org/A368875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*(3n + 1)!/(n!*((n + 2)!)^2) (OEIS A368875)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 6 */
    seq_t res = 6;
    if (n == 0) return res;

    for (idx_t k = 0; k < n; ++k)        /* compute successive terms */
    {
        /* denominator factors: (k+1) * (k+3)^2   -> stored as three separate factors */
        seq_t denom[3];
        denom[0] = (seq_t)(k + 1);
        denom[1] = (seq_t)(k + 3);
        denom[2] = (seq_t)(k + 3);

        /* numerator factors: (3k+2), (3k+3), (3k+4) */
        seq_t num[3];
        num[0] = (seq_t)(3 * k + 2);
        num[1] = (seq_t)(3 * k + 3);
        num[2] = (seq_t)(3 * k + 4);

        for (int i = 0; i < 3; ++i)
        {
            /* cancel any denominator factor that divides the current result
               before the multiplication to keep intermediate values small */
            for (int j = 0; j < 3; ++j)
            {
                if (denom[j] == 1) continue;
                if ((res % denom[j]) == 0)
                {
                    res /= denom[j];
                    denom[j] = 1;
                }
            }

            /* multiply by current numerator factor */
            res *= num[i];

            /* cancel remaining denominator factors using gcd */
            for (int j = 0; j < 3; ++j)
            {
                if (denom[j] == 1) continue;
                seq_t g = gcd(res, denom[j]);
                if (g > 1)
                {
                    res /= g;
                    denom[j] /= g;
                }
            }
        }
        /* At this point all denominator factors should be cancelled (denom[j]==1). */
    }
    return res;
}
