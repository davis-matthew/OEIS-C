/*
 * A375676  a(n) is the perimeter of the rectangle whose edges are n and
 *          A375673(n).
 * https://oeis.org/A375676
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n + m), m = A375673(n) = min{ m >= n : n*m divisible
 *            by 2*(n+m) }, found by search over t = area/perimeter ratio
 * Range:     u8 n <= 12, u16 n <= 190, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 902 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t best_m = 0;
    if ((n & 3ULL) == 0) {
        best_m = N;
    } else {
        idx_t max_t = (n - 1) / 2;
        for (idx_t t = 1; t <= max_t; ++t) {
            seq_t denom = (seq_t)(n - 2 * t);
            seq_t numer = (seq_t)2 * (seq_t)t * N;
            if (numer % denom == 0) {
                seq_t m = numer / denom;
                if (m >= N && (best_m == 0 || m < best_m)) {
                    best_m = m;
                }
            }
        }
    }
    return (seq_t)2 * (N + best_m);
}
