/*
 * A332351  Triangle read by rows: T(m,n) = Sum_{-m<i<m, -n<j<n, gcd{i,j}=1}
 *          (m-|i|)*(n-|j|)/2, m >= n >= 1.
 * https://oeis.org/A332351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) where m and n are obtained by reading the triangle
 *            by rows. T(m,n) = \sum_{-m<i<m, -n<j<n, gcd(i,j)=1}
 *            (m-|i|)*(n-|j|)/2 with offset 1; i.e. a(1)=T(1,1)=0
 * Range:     u8 n <= 19, u16 n <= 252, u32 n <= 8364, u64 n <= 8364,
 *            u128 n <= 8364, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* find row m such that cumulative count up to row m-1
       is < n <= cumulative up to m. */
    idx_t m = 1;
    __uint128_t cum;
    while (1) {
        cum = (__uint128_t)m * (m + 1) / 2;
        if (cum >= n) break;
        ++m;
    }
    /* previous cumulative count */
    idx_t prev_cum = (m - 1) * m / 2;
    /* column index within the row (1‑based) */
    idx_t col = n - prev_cum;   /* 1 <= col <= m */

    seq_t total = 0;
    for (idx_t i = 0; i < m; ++i) {
        for (idx_t j = 0; j < col; ++j) {
            /* skip the (0,0) pair; gcd(0,0)=0 so it's omitted naturally */
            if (i == 0 && j == 0) continue;
            /* check coprimality */
            if (gcd((seq_t)i, (seq_t)j) != 1) continue;

            /* determine multiplier: 4 for i>0 && j>0, 2 if exactly one zero */
            int mult = (i == 0 || j == 0) ? 2 : 4;

            seq_t term = (seq_t)((m - i) * (col - j) * mult / 2);
            total += term;
        }
    }
    return total;
}
