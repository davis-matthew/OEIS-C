/*
 * A282083  Number of n-element subsets of [n+7] having an even sum.
 * https://oeis.org/A282083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n‑element subsets of [n+7] having an even sum.
 *            Computation: For m = n+7 If n is odd: a(n) = C(m,n) / 2 If n is
 *            even: t = (n+6)/2 // floor(m/2) h = n/2 a(n) = (C(m,n) + (-1)^h
 *            * C(t,h)) / 2 where C(x,y) is the binomial coefficient
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    // choose(m, n)
    seq_t c_m_n = binom(n + 7, n);

    if (n & 1) /* n is odd */
    {
        return c_m_n / 2;
    }
    else /* n is even */
    {
        idx_t half = n / 2;
        seq_t c_t_half = binom((n + 6) / 2, half);

        if (half & 1) /* sign = -1 (odd half) */
            return (c_m_n - c_t_half) / 2;
        else          /* sign = +1 (even half) */
            return (c_m_n + c_t_half) / 2;
    }
}
