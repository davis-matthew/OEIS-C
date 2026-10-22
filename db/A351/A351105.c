/*
 * A351105  a(n) = Sum_{k=1..n} Sum_{j=1..k} Sum_{i=1..j} (i*j*k)^2.
 * https://oeis.org/A351105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+6,7)*(35*n*n-21*n+4)/144
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 242, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t poly = 35 * sn * sn - 21 * sn + 4;

    /* Compute binomial(2*n+6, 7) */
    idx_t top  = 2 * n + 6;
    seq_t binom = 1;

    for (idx_t k = 1; k <= 7; ++k) {
        seq_t numer = (seq_t)(top - (k - 1));
        seq_t denom = (seq_t)k;

        seq_t g = gcd(numer, denom);
        numer /= g;
        denom /= g;

        if (denom != 1) {
            g = gcd(binom, denom);
            binom /= g;
            denom /= g;
        }
        binom *= numer;
    }

    /* Cancel the factor 144 = 2^4 * 3^2 from binom * poly */
    const int denom_int[6] = {2, 2, 2, 2, 3, 3};
    seq_t num  = binom;
    seq_t mult = poly;

    for (int i = 0; i < 6; ++i) {
        seq_t d = (seq_t)denom_int[i];

        seq_t g = gcd(num, d);
        num /= g;
        d   /= g;

        if (d != 1) {
            g = gcd(mult, d);
            mult /= g;
            d   /= g;
        }
    }

    return num * mult;
}
