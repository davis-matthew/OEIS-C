/*
 * A325499  Difference sequence of A325424.
 * https://oeis.org/A325499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A325424(n+1) - A325424(n), where A325424 lists integers
 *            whose 2-adic plus 3-adic valuations sum to an odd number (found
 *            by direct scan)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9435 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A325499_odd(uint64_t v) {
    int parity = 0;
    while (v % 2 == 0) { v /= 2; parity ^= 1; }
    while (v % 3 == 0) { v /= 3; parity ^= 1; }
    return parity;
}

seq_t a(idx_t n) {
    uint64_t cnt = 0;
    uint64_t prev = 0, cur = 0;
    uint64_t v;
    if (n == 0) return 0;
    for (v = 2; cnt < (uint64_t)n + 1; v++) {
        if (A325499_odd(v)) {
            cnt++;
            if (cnt == (uint64_t)n) prev = v;
            if (cnt == (uint64_t)n + 1) cur = v;
        }
    }
    return (seq_t)(cur - prev);
}
