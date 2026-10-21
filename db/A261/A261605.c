/*
 * A261605  G.f.: Sum_{n=-oo..+oo} x^(n^2) / (1 - x^n)^n.
 * https://oeis.org/A261605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d|n} ((-1)^(n/d + d) * binomial(d, n/d) +
 *            binomial(n/d - 1, d - 1)), with a(0) = 1
 * Range:     i8 n <= 23, i16 n <= 103, i32 n <= 428, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {                 /* offset 0 */
        return 1;
    }

    seq_t result = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t m = n / d;      /* m = n / d */

            /* contribution of divisor d */
            seq_t term = binomial(d, m);
            if ((m + d) & 1)       /* (-1)^(m+d) */
                term = -term;
            term += binomial(m - 1, d - 1);
            result += term;

            /* if d and m are distinct, also include divisor m */
            if (d != m) {
                idx_t d2 = m;
                idx_t m2 = d;     /* n / d2 = d */

                seq_t term2 = binomial(d2, m2);
                if ((m2 + d2) & 1)
                    term2 = -term2;
                term2 += binomial(m2 - 1, d2 - 1);
                result += term2;
            }
        }
    }

    return result;
}
