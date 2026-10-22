/*
 * A323675  G.f.: Sum_{n>=0} x^n * (x^n + i)^n / (1 + i*x^(n+1))^(n+1), where
 *          i^2 = -1.
 * https://oeis.org/A323675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Sum_{m>=0} x^m * (x^m + i)^m / (1 +
 *            i*x^(m+1))^(m+1), where i^2 = -1
 * Range:     i8 n <= 21, i16 n <= 68, i32 n <= 214, i64 n <= 769,
 *            i128 n <= 2848, big any n
 * Helpers:   binom_c
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_c.h"

seq_t a(idx_t n)
{
    /* Input index */
    idx_t N = n;
    seq_t total = 0;

    for (idx_t m = 0; m <= N; ++m) {
        /* Loop over n in the outer sum of the generating function */
        seq_t bin1 = 1;          /* C(m,0) initially */

        for (idx_t a = 0; a <= m; ++a) {
            /* Compute sub = m * (1 + a) */
            idx_t sub = m * (1 + a);
            if (N < sub)          /* further a will give larger sub */
                break;

            idx_t rem = N - sub;
            if (rem % (m + 1) != 0)
                ;                 /* no valid k for this a */
            else {
                idx_t k = rem / (m + 1);
                /* e = m - a + k must be even for real contribution */
                idx_t e = m - a + k;
                if ((e & 1) == 0) {
                    /* binomial coefficients */
                    seq_t bin2 = binom_c(m + k, k);         /* C(m+k, k) = C(m+k, m) */
                    seq_t term = bin1 * bin2;

                    /* Sign from (-1)^k * (-1)^{e/2} */
                    idx_t exp_half = e >> 1;
                    if (((k + exp_half) & 1) != 0)
                        term = -term;

                    total += term;
                }
            }

            /* Update bin1 for next a: C(m,a+1) = C(m,a) * (m-a)/(a+1) */
            if (a < m) {
                bin1 = bin1 * (seq_t)(m - a) / (seq_t)(a + 1);
            }
        }
    }

    return total;
}
