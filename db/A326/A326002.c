/*
 * A326002  G.f.: Sum_{n>=0} (n+1) * x^n * (1 + x^n)^n.
 * https://oeis.org/A326002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum_{m|n, n/m <= m+1} (m+1) * C(m, n/m - 1)
 * Range:     u8 n <= 23, u16 n <= 83, u32 n <= 359, u64 n <= 1439,
 *            u128 n <= 6005, big any n
 * Helpers:   binom_m_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_m_n.h"
/* (plus a(0)=1) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        idx_t m1 = d;          /* first divisor */
        idx_t t1 = n / d;      /* corresponding t = n/m */

        if (t1 <= m1 + 1)
        {
            seq_t bin = binom_m_n(m1, t1 - 1);
            sum += ((seq_t)(m1 + 1)) * bin;
        }

        idx_t m2 = n / d;      /* second divisor, if distinct */
        if (m2 != m1)
        {
            idx_t t2 = d;      /* n/m2 */
            if (t2 <= m2 + 1)
            {
                seq_t bin = binom_m_n(m2, t2 - 1);
                sum += ((seq_t)(m2 + 1)) * bin;
            }
        }
    }
    return sum;
}
