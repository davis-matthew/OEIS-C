/*
 * A249728  After a(1) = 1 each n appears A000720(n) times.
 * https://oeis.org/A249728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = least m >= 2 such that 1 +
 *            Sum_{k=2..m} A000720(k) >= n (each m appears A000720(m) times,
 *            found by a running count)
 * Range:     u8 n <= 7744, u16 n <= 10016, u32 n <= 10016, u64 n <= 10016,
 *            u128 n <= 10016, big any n
 * Validated: standard, pass, 108 terms through n = 9877 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A249728_isprime(uint64_t m) {
    if (m < 2) return 0;
    if (m < 4) return 1;
    if (m % 2 == 0) return 0;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t cum = 1, pi = 0, m = 1;
    while (cum < n) {
        m++;
        if (A249728_isprime(m)) pi++;
        cum += pi;
    }
    return (seq_t)m;
}
