/*
 * A373101  Triangle read by rows, T(n,k) = (binomial(n,k)^3 -
 *          binomial(n,k))/6 for k=1..n-1 and n >= 2.
 * https://oeis.org/A373101
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,k) = (binomial(n,k)^3 - binomial(n,k))/6, read by rows with
 *            offset 2
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

static inline seq_t binom_mult(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;
    idx_t m = n - 1;               /* position in the flattened triangle, starting from 1 */
    idx_t disc = 8 * m + 1;
    idx_t sqrtDisc = floor_sqrt_u64(disc);
    idx_t r = (1 + sqrtDisc) / 2;   /* preliminary row estimate */
    if (r * (r - 1) / 2 < m) ++r;   /* correct to minimal row satisfying the inequality */
    idx_t prev = (r - 1) * (r - 2) / 2;
    idx_t k = m - prev;             /* column index within row r, 1 ≤ k ≤ r-1 */
    seq_t c = binom_mult(r, k);
    seq_t c2 = c * c;
    seq_t val = c2 * c - c;         /* c^3 - c */
    return val / 6;
}
