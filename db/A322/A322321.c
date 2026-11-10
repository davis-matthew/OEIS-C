/*
 * A322321  a(n) = lcm(A003557(n), A173557(n)).
 * https://oeis.org/A322321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(A003557(n), A173557(n)), where A003557(n) = n/rad(n)
 *            and A173557(n) = Product_{p|n} (p-1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9664 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A322321_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t x = 1; /* n / rad(n) */
    seq_t y = 1; /* product of (p-1) */
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            y *= (seq_t)(p - 1);
            m /= p;
            while (m % p == 0) {
                m /= p;
                x *= (seq_t)p;
            }
        }
    }
    if (m > 1) {
        y *= (seq_t)(m - 1);
    }
    seq_t g = A322321_gcd(x, y);
    return (x / g) * y;
}
