/*
 * A318509  Completely multiplicative with a(p) = A002487(p).
 * https://oeis.org/A318509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} A002487(p)^e (completely
 *            multiplicative), a(1) = 1, where A002487 is Stern's diatomic
 *            series
 * Range:     u8 n <= 3414, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9733 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A318509_stern(uint64_t n) {
    seq_t x = 1, y = 0;
    while (n > 0) {
        if (n & 1) y += x;
        else x += y;
        n >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    while ((m & 1) == 0 && m > 0) {
        result *= A318509_stern(2);
        m >>= 1;
    }
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            seq_t s = A318509_stern(p);
            while (m % p == 0) {
                result *= s;
                m /= p;
            }
        }
    }
    if (m > 1) result *= A318509_stern(m);
    return result;
}
