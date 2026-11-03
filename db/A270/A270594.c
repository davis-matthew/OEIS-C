/*
 * A270594  Number of ordered ways to write n as the sum of a triangular
 *          number, a positive square and the square of a generalized
 *          pentagonal number (A001318).
 * https://oeis.org/A270594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (x≥0, y≥1, z∈ℤ) such that * n =
 *            T(x) + y² + pen(z)², * where T(x) = x(x+1)/2 and pen(z) =
 *            z(3z−1)/2 (generalized pentagonal * numbers). The procedure
 *            enumerates all pentagonal numbers whose square * does not exceed
 *            n−1, then tests every positive square and checks...
 * Range:     u8 n <= 8, u16 n <= 1858, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9885 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; guard against n==0 */

    seq_t N = (seq_t)n;
    seq_t count = 0;

    for (idx_t k = 0; ; ++k)
    {
        /* pen_pos corresponds to z = k  (k≥0) */
        seq_t pen_pos = (seq_t)k * (((seq_t)3 * k) - 1) / 2;
        seq_t pen_pos_sq = pen_pos * pen_pos;

        int processed = 0;
        if (pen_pos_sq <= N - 1)
        {
            processed = 1;
            seq_t rem = N - pen_pos_sq;
            idx_t max_y = (idx_t)floor_sqrt(rem);
            for (idx_t y = 1; y <= max_y; ++y)
            {
                seq_t y2 = (seq_t)y * y;
                seq_t rem2 = rem - y2;               /* rem2 ≥ 0 */
                seq_t t = rem2 * 8 + 1;
                seq_t sqrt_t = floor_sqrt(t);
                if (sqrt_t * sqrt_t == t)
                    ++count;
            }
        }

        if (k > 0)
        {
            /* pen_neg corresponds to z = –k (k>0) */
            seq_t pen_neg = (seq_t)k * (((seq_t)3 * k) + 1) / 2;
            seq_t pen_neg_sq = pen_neg * pen_neg;
            if (pen_neg_sq <= N - 1)
            {
                processed = 1;
                seq_t rem = N - pen_neg_sq;
                idx_t max_y = (idx_t)floor_sqrt(rem);
                for (idx_t y = 1; y <= max_y; ++y)
                {
                    seq_t y2 = (seq_t)y * y;
                    seq_t rem2 = rem - y2;
                    seq_t t = rem2 * 8 + 1;
                    seq_t sqrt_t = floor_sqrt(t);
                    if (sqrt_t * sqrt_t == t)
                        ++count;
                }
            }
        }

        if (!processed) break;    /* both pen_pos_sq and pen_neg_sq > N−1 */
    }

    return count;
}
