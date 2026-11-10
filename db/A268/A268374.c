/*
 * A268374  a(n) = A001222(n) - A267116(n).
 * https://oeis.org/A268374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A001222(n) - A267116(n), i.e. Omega(n) minus the
 *            bitwise-OR of the prime exponents of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9457 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    unsigned total = 0, orv = 0;
    idx_t t = n;
    unsigned e = 0;
    while ((t & 1) == 0) { t >>= 1; e++; }
    total += e; orv |= e;
    for (idx_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            e = 0;
            while (t % p == 0) { t /= p; e++; }
            total += e; orv |= e;
        }
    }
    if (t > 1) { total += 1; orv |= 1; }
    return (seq_t)(total - orv);
}
