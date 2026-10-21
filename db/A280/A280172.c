/*
 * A280172  Lexicographically earliest table of positive integers read by
 *          antidiagonals such that no row or column contains a repeated term.
 * https://oeis.org/A280172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = ((i-1) XOR (j-1)) + 1, where (i, j) are the coordinates
 *            in the antidiagonal enumeration of the table T(i,j) = ((i-1) XOR
 *            (j-1)) + 1
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 9763, u64 n <= 9763,
 *            u128 n <= 9763, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t mid_sq = (seq_t)mid * mid;
        if (mid_sq <= (seq_t)n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, defensive return */

    /* Find diagonal index k such that t(k-1) < n <= t(k).  */
    idx_t s = floor_sqrt_u64(1 + 8 * n);
    idx_t k = (s - 1) / 2;
    seq_t tri = (seq_t)k * (k + 1) / 2;
    if (tri < (seq_t)n) ++k;

    /* Compute m = n - t(k-1), coordinates i and j.  */
    idx_t t_prev_idx = (idx_t)((seq_t)k * (k - 1) / 2);
    idx_t m = (idx_t)((seq_t)n - t_prev_idx);      /* 1 <= m <= k   */
    idx_t i = k - m + 1;                            /* row index   */

    /* Value at (i, m).  */
    seq_t val = ((seq_t)(i - 1) ^ (seq_t)(m - 1)) + 1;
    return val;
}
