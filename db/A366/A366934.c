/*
 * A366934  Expansion of Sum_{k>=1} k^5 * x^k/(1 - x^k)^5.
 * https://oeis.org/A366934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^5 * binomial(n/d+3,4)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A366934 – Expansion of Sum_{k≥1} k^5 * x^k/(1−x^k)^5 */
#include <stddef.h>
#include <stdint.h>

static seq_t divisor_contribution(idx_t d, idx_t n)
{
    seq_t dv = (seq_t)d;
    seq_t d2 = dv * dv;          /* d^2 */
    seq_t d4 = d2 * d2;          /* d^4 */
    seq_t d5 = d4 * dv;          /* d^5 */

    idx_t m_idx = n / d + 3;     /* m = n/d + 3 */
    seq_t m   = (seq_t)m_idx;
    seq_t bin = m * (m - 1);     /* m*(m-1) */
    bin = bin * (m - 2);         /* * (m-2) */
    bin = bin * (m - 3);         /* * (m-3) */
    bin = bin / 24;              /* divide by 4! = 24 */

    return d5 * bin;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            seq_t c1 = divisor_contribution(i, n);
            sum += c1;

            idx_t j = n / i;
            if (j != i)
            {
                seq_t c2 = divisor_contribution(j, n);
                sum += c2;
            }
        }
    }
    return sum;
}
