/*
 * A385108  Triangle a(n,k) read by antidiagonals: a(n,k) is the number of
 *          dots in the k-augmented centered triangle of order n, k>=0, n>=1.
 * https://oeis.org/A385108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) maps the index n to the triangle a(row,k) read by
 *            antidiagonals. For each antidiagonal with sum s = row + k, the
 *            pairs are listed by increasing row (1..s). a(row,0) = (3*row^2 -
 *            3*row + 2)/2. Recurrence: a(row,k) = 4*a(row,k-1) -
 *            3*(2^(k-1)*row - (2^(k-1)-1))
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 154, u64 n <= 562,
 *            u128 n <= 1000, big any n
 * Helpers:   triangular_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/triangular_num.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find smallest s with s*(s+1)/2 >= n */
    idx_t lo = 0;
    idx_t hi = 1;
    while (triangular_num(hi) < n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular_num(mid) < n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t s = hi;
    idx_t cum_before = triangular_num(s - 1);
    idx_t pos = n - cum_before - 1;       /* 0‑based position in this antidiagonal */
    seq_t row = (seq_t)(pos + 1);
    seq_t k   = (seq_t)(s - row);

    /* Base value a(row,0) */
    seq_t val = ((seq_t)3 * row * row - (seq_t)3 * row + (seq_t)2) / (seq_t)2;

    seq_t p2 = (seq_t)1;          /* 2^(j-1) */
    seq_t n_minus_1 = row - (seq_t)1;

    for (idx_t j = 1; j <= k; ++j) {
        seq_t f = p2 * n_minus_1 + (seq_t)1;
        val = (seq_t)4 * val - (seq_t)3 * f;
        p2 <<= 1;
    }

    return val;
}
