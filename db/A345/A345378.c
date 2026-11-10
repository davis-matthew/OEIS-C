/*
 * A345378  Number of terms m <= n, where m is a term in A006497.
 * https://oeis.org/A345378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of k >= 0 with A006497(k) <= n, where A006497(k) =
 *            3*A006497(k-1) + A006497(k-2), A006497(0) = 2, A006497(1) = 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 9592 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n;
    if (x < 2) return (seq_t)0;
    if (x < 3) return (seq_t)1;
    uint64_t count = 2;
    uint64_t p2 = 2, p1 = 3;
    for (;;) {
        if (p1 > (UINT64_MAX - p2) / 3) break;
        uint64_t c = 3 * p1 + p2;
        if (c > x) break;
        count++;
        p2 = p1;
        p1 = c;
    }
    return (seq_t)count;
}
