/*
 * A290744  Maximum number of distinct Lyndon factors that can appear in words
 *          of length n over an alphabet of size 5.
 * https://oeis.org/A290744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+1,2) - (5-p)*binomial(m+1,2) -
 *            p*binomial(m+2,2) + 5 where m = floor(n/5), p = n mod 5. This
 *            implements OEIS A290744
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n>=1 */
    seq_t N   = (seq_t)n;
    seq_t binom_n = N * (N + 1) / 2;           /* binomial(n+1,2) */

    idx_t m_idx = n / 5;                       /* floor(n/5) */
    seq_t m = (seq_t)m_idx;

    idx_t p_idx = n - m_idx * 5;               /* n mod 5 */
    seq_t p = (seq_t)p_idx;

    seq_t binom_m1 = m * (m + 1) / 2;           /* binomial(m+1,2) */
    seq_t binom_m2 = (m + 1) * (m + 2) / 2;     /* binomial(m+2,2) */

    seq_t sp = (seq_t)5 - p;                    /* s-p with s=5 */

    seq_t result = binom_n
                   - sp * binom_m1
                   - p  * binom_m2
                   + (seq_t)5;                 /* +s */

    return result;
}
