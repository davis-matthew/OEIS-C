/*
 * A273927  Absolute difference between A000290(n) and the nearest term of
 *          A000578.
 * https://oeis.org/A273927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over cubes c != n^2 of |n^2 - c|, with c = k^3, k >=
 *            0 (the nearest cube other than n^2 itself); computed by an
 *            integer cube root
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool A273927_ok(seq_t k, seq_t m) {
    /* k^3 <= m, k >= 1 */
    return k <= m / (k * k);
}

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * (seq_t)n;
    seq_t hi = 1, lo;
    while (A273927_ok(hi, m)) hi *= 2;
    lo = hi / 2;
    if (lo == 0) lo = 0;
    /* invariant: lo valid (or 0), hi invalid */
    while (hi - lo > 1) {
        seq_t mid = lo + (hi - lo) / 2;
        if (A273927_ok(mid, m)) lo = mid; else hi = mid;
    }
    seq_t k = lo;
    seq_t f = k * k * k;
    seq_t k1 = k + 1;
    seq_t c = k1 * k1 * k1;
    seq_t d1, d2;
    if (f == m) {
        seq_t p = k - 1;
        d1 = m - p * p * p;
        d2 = c - m;
    } else {
        d1 = m - f;
        d2 = c - m;
    }
    return d1 < d2 ? d1 : d2;
}
