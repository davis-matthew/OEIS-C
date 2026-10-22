/*
 * A329444  The sixth moments of the squared binomial coefficients; a(n) =
 *          Sum_{m=0..n} m^6*binomial(n, m)^2.
 * https://oeis.org/A329444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0..n} m^6 * binomial(n,m)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 22, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;           /* binomial(n,0) */
    for (idx_t m = 0; m <= n; ++m) {
        seq_t m_val = (seq_t)m;

        /* compute m^6 */
        seq_t m_pow = 1;
        for (int i = 0; i < 6; ++i) {
            m_pow *= m_val;
        }

        /* binomial(n,m)^2 */
        seq_t binSq = bin;
        binSq *= bin;

        sum += m_pow * binSq;

        if (m < n) {
            /* update bin to binomial(n,m+1) */
            seq_t t = bin * (seq_t)(n - m);
            bin = t / (m + 1);
        }
    }
    return sum;
}
