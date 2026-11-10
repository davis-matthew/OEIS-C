/*
 * A302694  a(n) is the smallest integer k such that A002828(k*n) = 3.
 * https://oeis.org/A302694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k >= 1 such that A002828(k*n) = 3; found by
 *            search over k
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

static int A302694_two_squares(uint64_t m) {
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

static int A002828_a(uint64_t m) {
    uint64_t r;
    if (m == 0) return 0;
    r = isqrt_u64(m);
    if (r * r == m) return 1;
    if (A302694_two_squares(m)) return 2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return 4;
    return 3;
}

seq_t a(idx_t n) {
    uint64_t k;
    for (k = 1;; k++) {
        if (A002828_a((uint64_t)n * k) == 3) return (seq_t)k;
    }
}
