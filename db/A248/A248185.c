/*
 * A248185  Numbers k such that A248183(k+1) = A248183(k) + 1.
 * https://oeis.org/A248185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(2*t*(t+1))) - 1 with t = n+1 (smallest m with
 *            A248183(m+1) >= n+1), found by integer binary search
 * Range:     u8 n <= 9, u16 n <= 205, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 910 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t t = (seq_t)n + 1;
    seq_t target = (seq_t)2 * t * (t + 1);
    seq_t lo = t;
    seq_t hi = t + t / 2 + 2;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (mid * mid >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo - 1;
}
