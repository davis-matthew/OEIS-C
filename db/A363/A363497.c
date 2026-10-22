/*
 * A363497  a(n) = Sum_{k=0..n} floor(sqrt(k))^3.
 * https://oeis.org/A363497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} floor(sqrt(k))^3 Closed form from the OEIS
 *            entry: a(n) = (n+1)*m^3 - m*(m+1)*(3m-1)*(12m^2+7m-4)/60, where
 *            m = floor(sqrt(n))
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 4399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_floor(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 mid_sq = (__int128)mid * mid;
        if (mid_sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t m = isqrt_floor(n);
    seq_t m_s = (seq_t)m;
    seq_t m3 = m_s * m_s * m_s;                 /* m^3 */
    seq_t term1 = (seq_t)(n + 1) * m3;          /* (n+1)*m^3 */

    seq_t m1 = m_s + 1;
    seq_t t1 = 3 * m_s - 1;                     /* 3m-1 */
    seq_t t2 = 12 * m_s * m_s + 7 * m_s - 4;    /* 12m^2+7m-4 */

    seq_t num = m_s * m1 * t1 * t2;             /* numerator of the fraction */
    seq_t term2 = num / 60;                     /* divide by 60; division is exact */

    return term1 - term2;
}
