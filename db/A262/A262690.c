/*
 * A262690  a(n) = largest square k <= n such that A002828(n-k) =
 *          A002828(n)-1.
 * https://oeis.org/A262690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest square k <= n with A002828(n-k) = A002828(n)-1;
 *            a(0) = 0 (search downward over k = r^2, r <= floor(sqrt(n)))
 * Range:     u8 n <= 255, u16 n <= 65535, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 117 terms through n = 9877 (2026-11-07)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

static int A262690_two_squares(uint64_t m) {
    uint64_t p;
    while (m % 2 == 0) m /= 2;
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (p % 4 == 3 && (e & 1)) return 0;
        }
    }
    if (m > 1 && m % 4 == 3) return 0;
    return 1;
}

static int A262690_A002828(uint64_t m) {
    uint64_t r;
    if (m == 0) return 0;
    r = isqrt_u64(m);
    if (r * r == m) return 1;
    if (A262690_two_squares(m)) return 2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return 4;
    return 3;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n, r;
    int target;
    if (m == 0) return (seq_t)0;
    target = A262690_A002828(m) - 1;
    for (r = isqrt_u64(m); r >= 1; r--) {
        if (A262690_A002828(m - r * r) == target)
            return (seq_t)(r * r);
    }
    return (seq_t)0;
}
