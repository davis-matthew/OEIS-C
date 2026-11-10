/*
 * A272120  Square array T(n,k), n>=1, k>=1, read by antidiagonals downwards
 *          in which column k lists the alternating row sums of the first k
 *          columns of the triangle A196020.
 * https://oeis.org/A272120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=1..k} (-1)^(j+1)*A196020(n,j), where
 *            A196020(n,j) = 2*((n-j(j+1)/2)/j)+1 if j(j+1)/2 <= n and j
 *            divides n-j(j+1)/2, else 0; array read by antidiagonals
 *            downwards: a(n) = T(i, d+1-i)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 95 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) d++;
    idx_t i = n - (d * (d - 1)) / 2;   /* 1..d, row index */
    idx_t row = i;
    idx_t col = d + 1 - i;

    seq_t pos = 0, neg = 0;
    for (idx_t j = 1; j <= col; j++) {
        idx_t t = j * (j + 1) / 2;
        if (t > row) break;
        idx_t diff = row - t;
        if (diff % j == 0) {
            seq_t v = (seq_t)2 * (seq_t)(diff / j) + 1;
            if (j & 1) pos += v; else neg += v;
        }
    }
    return pos - neg;
}
