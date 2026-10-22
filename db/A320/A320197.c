/*
 * A320197  Number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 9*d^2 = n.
 * https://oeis.org/A320197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + 2*b^2 + 4*c^2 +
 *            9*d^2 = n
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   coeff_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/coeff_square.h"
/* Integer floor square root for a 64‑bit unsigned integer */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (x >> 1) + 1;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* maximum |d|: 9*d^2 <= n  */
    idx_t dmax = isqrt_u64(n / 9);
    for (idx_t d = 0; d <= dmax; ++d) {
        idx_t d2   = coeff_square(d, 9);      /* 9*d^2   */
        idx_t dmult = d == 0 ? 1 : 2;          /* sign factor for d */
        idx_t rem_d = n - d2;

        idx_t cmax = isqrt_u64(rem_d / 4);
        for (idx_t c = 0; c <= cmax; ++c) {
            idx_t c2   = coeff_square(c, 4);   /* 4*c^2   */
            idx_t cmult = c == 0 ? 1 : 2;
            idx_t rem_c = rem_d - c2;

            idx_t bmax = isqrt_u64(rem_c / 2);
            for (idx_t b = 0; b <= bmax; ++b) {
                idx_t b2   = coeff_square(b, 2);   /* 2*b^2   */
                idx_t bmult = b == 0 ? 1 : 2;
                idx_t rem_b = rem_c - b2;

                idx_t a_abs = isqrt_u64(rem_b);
                if (a_abs * a_abs != rem_b)
                    continue;                     /* not a perfect square */

                idx_t amult = a_abs == 0 ? 1 : 2;
                total += (seq_t)dmult * cmult * bmult * amult;
            }
        }
    }

    return total;
}
