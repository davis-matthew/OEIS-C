/*
 * A253902  Write numbers 1, then 2^2 down to 1, then 3^2 down to 1, then 4^2
 *          down to 1 and so on.
 * https://oeis.org/A253902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = k^2 - (n - S(k-1)) where k = minimal integer ≥ 1 with
 *            S(k) = k*(k+1)*(2k+1)/6 ≥ n
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: Uses only integer arithmetic (no floating point). */

#include <stdint.h>

static seq_t sum_squares(idx_t k) {
    /* Return sum of squares 1^2 + 2^2 + … + k^2 = k(k+1)(2k+1)/6 */
    seq_t kk = k;
    return kk * (kk + 1) * (2 * kk + 1) / 6;
}

static idx_t find_k(idx_t n) {
    /* Binary search for k: S(k-1) < n ≤ S(k) */
    idx_t low = 1, high = 1;

    /* Find an upper bound for k */
    while (sum_squares(high) < (seq_t)n && high < UINT64_MAX / 2) {
        high <<= 1;          /* high *= 2 */
    }
    if (sum_squares(high) < (seq_t)n) {
        /* Fallback: use n as an upper bound (safe for very small n) */
        high = n;
    }

    /* Binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (sum_squares(mid) >= (seq_t)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;   /* low == high */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* safety (not required by OEIS offset) */

    idx_t k = find_k(n);
    seq_t kk = k;

    /* sum of squares up to k-1 */
    seq_t prev_sum = sum_squares(k - 1);

    /* position within the k-th block (1‑based) */
    seq_t pos = (seq_t)n - prev_sum;

    /* value in the descending block */
    seq_t val = kk * kk - (pos - 1);
    return val;
}
