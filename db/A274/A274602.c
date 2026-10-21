/*
 * A274602  Triangle read by rows: T(n,k) = k*(n-k+1)^2 + n - k, 0 <= k <= n.
 * https://oeis.org/A274602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k*(r-k+1)^2 + r - k, where r is the row number (0‑based)
 *            satisfying (r+1)(r+2)/2 ≥ n, and k = n - r(r+1)/2 - 1 is the
 *            column index (0‑based) in that row. The sequence a(n) is the
 *            flattening of the triangular array T(r,k) defined for 0 ≤ k ≤ r
 *            by T(r,k) = k*(r-k+1)^2 + r - k
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1, but guard against 0 */
    idx_t t = n - 1;                 /* zero‑based position in the triangle */
    idx_t r = 0;
    idx_t cumulative = 0;
    /* find r such that cumulative + (r+1) > t */
    while (cumulative + r + 1 <= t) {
        cumulative += r + 1;
        ++r;
    }
    idx_t k = t - cumulative;        /* column index in row r */

    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;
    seq_t diff = rn - kn + 1;        /* (r-k+1) */
    seq_t val = kn * (diff * diff) + (rn - kn);
    return val;
}
