/*
 * A378234  From higher-order arithmetic progressions: Corrected version of
 *          A259461.
 * https://oeis.org/A378234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+4)^3*(n+5))/(2*n*(n+2)) * a(n-1), a(0) = 40
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)40;

    seq_t acc = (seq_t)40;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t koff = (seq_t)k;
        seq_t n4   = koff + 4;
        seq_t n5   = koff + 5;

        /* α = (n+4)^3 * (n+5) */
        seq_t alpha = n4 * n4 * n4 * n5;

        /* β = 2 * n * (n+2) */
        seq_t beta  = (seq_t)2 * koff * (koff + 2);

        /* Reduce the fraction acc * α / β to avoid overflow */
        seq_t g = gcd(acc, beta);
        acc /= g;
        beta /= g;

        g = gcd(alpha, beta);
        alpha /= g;
        beta /= g;

        acc = acc * alpha;
        if (beta != 1)
            acc /= beta;
    }

    return acc;
}
