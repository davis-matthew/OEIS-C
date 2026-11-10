/*
 * A248184  Numbers k such that A248183(k+1) = A248183(k).
 * https://oeis.org/A248184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k with A248183(k+1) = A248183(k), where A248183(n) =
 *            least k with 2*(k+1)*(k+2) > n^2. Computed by binary search:
 *            #{j<=m : A248183(j+1)=A248183(j)} = m - A248183(m+1) + 1
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 74 terms through n = 495 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A248184_f(idx_t n) {
    if (n <= 1) return 1;
    seq_t n_sq = (seq_t)n * (seq_t)n;
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t prod = (seq_t)(mid + 1) * (seq_t)(mid + 2);
        prod *= (seq_t)2;
        if (prod > n_sq) high = mid;
        else low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t lo = 1, hi = 4 * n + 4;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t cnt = mid - A248184_f(mid + 1) + 1;
        if (cnt >= n) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)lo;
}
