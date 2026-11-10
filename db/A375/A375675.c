/*
 * A375675  a(n) is the area of the rectangle whose edges are n and
 *          A375673(n).
 * https://oeis.org/A375675
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A375673(n), where A375673(n) = min{ m >= n : n*m is a
 *            multiple of 2*(n+m) }, found by searching m = 2*t*n/(n-2*t) over
 *            t = 1..floor((n-1)/2) (n divisible by 4 gives m = n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 899 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A375673_a(idx_t n) {
    if ((n & 3ULL) == 0) {
        return (seq_t)n;
    }
    seq_t best_m = 0;
    idx_t max_t = (n - 1) / 2;
    for (idx_t t = 1; t <= max_t; ++t) {
        idx_t denom_u = n - 2 * t;
        seq_t denom = (seq_t)denom_u;
        seq_t numer = (seq_t)2 * (seq_t)t * (seq_t)n;
        if (numer % denom == 0) {
            seq_t m = numer / denom;
            if (m >= (seq_t)n) {
                if (best_m == 0 || m < best_m) {
                    best_m = m;
                }
            }
        }
    }
    return best_m;
}

seq_t a(idx_t n) {
    return (seq_t)n * A375673_a(n);
}
