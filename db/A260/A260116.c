/*
 * A260116  G.f.: Sum_{n=-oo..+oo} (x + x^n)^n.
 * https://oeis.org/A260116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1 + Σ_{m≥1} [ x^m·(1 + x^{m-1})^m +
 *            x^{m^2}/(1 + x^{m+1})^m ]
 * Range:     u8 n <= 35, u16 n <= 133, u32 n <= 480, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pos = 0, neg = 0;

    /* First part: x^m·(1 + x^{m-1})^m */
    for (idx_t m = 1; m <= n; ++m) {
        if (m == 1) {
            /* (x + x)^1 = 2x, contributes only to a(1) */
            if (n == 1)
                pos += (seq_t)2;
            continue;
        }

        if (n < m)
            continue;

        idx_t diff = n - m;        /* n - m */
        idx_t denom = m - 1;       /* m - 1 > 0 for m ≥ 2 */

        if (diff % denom == 0) {
            idx_t k = diff / denom;
            if (k <= m) {
                pos += binomial((idx_t)m, (idx_t)k);
            }
        }
    }

    /* Second part: x^{m^2}/(1 + x^{m+1})^m */
    for (idx_t m = 1; m <= n / m; ++m) {   /* ensures m*m ≤ n and no overflow */
        if (n < m * m)
            continue;

        idx_t rem = n - m * m;           /* n - m^2 */
        idx_t step = m + 1;              /* exponent step */

        if (rem % step == 0) {
            idx_t k = rem / step;        /* k ≥ 0 */
            seq_t coeff = binomial(m + k - 1, k);

            if ((k & 1) == 0)           /* even k → positive sign */
                pos += coeff;
            else                        /* odd k → negative sign */
                neg += coeff;
        }
    }

    return pos - neg;
}
