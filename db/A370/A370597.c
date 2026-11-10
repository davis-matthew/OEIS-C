/*
 * A370597  Even numbers k such that gcd(k, A007814(k)) = 1.
 * https://oeis.org/A370597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th even k with gcd(k, A007814(k)) = 1; computed by a
 *            linear search over even k
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8531 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A370597_gcd(idx_t x, idx_t y) {
    while (y) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        k += 2;
        idx_t m = k;
        idx_t v = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            v++;
        }
        if (A370597_gcd(k, v) == 1)
            count++;
    }
    return (seq_t)k;
}
