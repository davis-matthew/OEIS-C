/*
 * A283974  Numbers n for which A002487(n-1) AND A002487(n) > 0 where AND is
 *          bitwise-and (A004198).
 * https://oeis.org/A283974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 1 with A002487(m-1) AND A002487(m) > 0; found
 *            by a linear search using the iterative Stern diatomic
 *            computation
 * Range:     u8 n <= 183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 7472 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A002487_stern(uint64_t n) {
    uint64_t x = 1, y = 0;
    while (n) {
        if (n & 1) y += x; else x += y;
        n >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t m = 0;
    uint64_t prev = 0; /* A002487(0) */
    while (count < n) {
        m++;
        uint64_t cur = A002487_stern(m);
        if ((prev & cur) > 0) count++;
        prev = cur;
    }
    return (seq_t)m;
}
