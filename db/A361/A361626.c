/*
 * A361626  Expansion of e.g.f. exp( x/(1-x)^3 ) / (1-x)^2.
 * https://oeis.org/A361626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (n+2*k+1)!/(3*k+1)! * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return (seq_t)1;

    /* term_0 = (n+1)! */
    seq_t term = 1;
    for (idx_t i = 2; i <= n + 1; ++i)
        term *= (seq_t)i;

    seq_t sum = term;            /* a(0) is included */

    /* Iterate k = 0 .. n-1 to generate term_{k+1} */
    for (idx_t k = 0; k < n; ++k)
    {
        /* Numerator and denominator factors for the multiplier */
        seq_t nums[3];
        nums[0] = (seq_t)(n + 2 * k + 3);   /* n+2k+3 */
        nums[1] = (seq_t)(n + 2 * k + 2);   /* n+2k+2 */
        nums[2] = (seq_t)(n - k);           /* n-k */

        seq_t dens[4];
        dens[0] = (seq_t)(k + 1);           /* k+1 */
        dens[1] = (seq_t)(3 * k + 4);       /* 3k+4 */
        dens[2] = (seq_t)(3 * k + 3);       /* 3k+3 */
        dens[3] = (seq_t)(3 * k + 2);       /* 3k+2 */

        /* Cancel common factors between term and denominators */
        seq_t tmp = term;
        for (int j = 0; j < 4; ++j)
        {
            if (dens[j] == 1) continue;
            seq_t g = gcd(tmp, dens[j]);
            if (g > 1)
            {
                tmp /= g;
                dens[j] /= g;
            }
        }

        /* Cancel remaining denominator factors with numerators */
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (dens[j] == 1) continue;
                seq_t g = gcd(nums[i], dens[j]);
                if (g > 1)
                {
                    nums[i] /= g;
                    dens[j] /= g;
                }
            }
        }

        /* After cancellation, all dens[j] should be 1.
           Multiply the remaining numerator factors into tmp. */
        for (int i = 0; i < 3; ++i)
            tmp *= nums[i];

        /* New term */
        term = tmp;
        sum += term;
    }

    return sum;
}
