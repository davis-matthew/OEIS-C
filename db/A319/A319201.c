/*
 * A319201  Expansion of f(t) = F^{[-1]}(t)/t, where F(x) = x/(1 - x^2 - x^3).
 * https://oeis.org/A319201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1/(n+1) * sum_{2*e2 + 3*e3 = n} (-1)^(e2+e3) * C(n+1,
 *            e2+e3) * C(e2+e3, e2) This follows from the binomial expansion
 *            of (1 - x^2 - x^3)^(n+1)
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 27, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t e3 = 0; e3 <= n / 3; ++e3)
    {
        idx_t rem = n - 3 * e3;
        if (rem & 1)          /* rem must be even */
            continue;

        idx_t e2   = rem / 2;
        idx_t e_sum = e2 + e3;

        seq_t term = binomial(n + 1, e_sum);
        term *= binomial(e_sum, e2);

        if (e_sum & 1)        /* odd exponent -> negative sign */
            term = -term;

        sum += term;
    }

    return sum / (seq_t)(n + 1);
}
