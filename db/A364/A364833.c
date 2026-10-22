/*
 * A364833  G.f. satisfies A(x) = 1 + x*A(x)^2/(1 - x^3*A(x)^3).
 * https://oeis.org/A364833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n-2*k-1,k) *
 *            binomial(2*n-3*k+1,n-3*k)/(2*n-3*k+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* a(0) = 1 */

    seq_t sum = 0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t a1 = n - 2 * k - 1;
        idx_t k1 = k;
        idx_t a2 = 2 * n - 3 * k + 1;
        idx_t k2 = n - 3 * k;

        seq_t bin1 = binomial(a1, k1);
        seq_t bin2 = binomial(a2, k2);
        seq_t d    = a2;             /* denominator: 2*n-3*k+1 */

        /* Reduce d with bin1 */
        seq_t g = gcd(bin1, d);
        if (g > 1) {
            bin1 /= g;
            d   /= g;
        }

        /* Reduce remaining d with bin2 */
        if (d > 1) {
            g = gcd(bin2, d);
            if (g > 1) {
                bin2 /= g;
                d   /= g;
            }
        }

        /* Final cleanup: cancel any leftover factors */
        while (d > 1) {
            g = gcd(bin1, d);
            if (g > 1) {
                bin1 /= g;
                d   /= g;
                continue;
            }
            g = gcd(bin2, d);
            if (g > 1) {
                bin2 /= g;
                d   /= g;
                continue;
            }
            /* In theory d should always become 1 because the fraction is integer */
            break;
        }

        /* Now bin1 * bin2 is the desired term */
        seq_t term = bin1 * bin2;
        sum += term;
    }

    return sum;
}
