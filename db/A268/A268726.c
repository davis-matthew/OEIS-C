/*
 * A268726  Index of the toggled bit between n and A268717(n+1): a(n) =
 *          A000523(A003987(n, A268717(1+n))).
 * https://oeis.org/A268726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007814(1 + A006068(n)), where A006068 is the inverse
 *            Gray code
 * Range:     u8 n <= 16383, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 137 terms through n = 9598 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t g = (uint64_t)n;
    uint64_t mask;
    for (mask = g >> 1; mask; mask >>= 1) {
        g ^= mask;
    }
    /* g = A006068(n); now 2-adic valuation of g + 1 */
    uint64_t v = g + 1;
    if (v == 0) return (seq_t)64;
    seq_t count = 0;
    while ((v & 1) == 0) {
        count++;
        v >>= 1;
    }
    return count;
}
