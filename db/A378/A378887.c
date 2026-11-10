/*
 * A378887  a(n) = gcd(n, A001511(n)).
 * https://oeis.org/A378887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, A001511(n)), where A001511(n) = 1 + (exponent of
 *            highest power of 2 dividing n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9770 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k = 1;
    idx_t m = n;
    while ((m & 1) == 0) {
        k++;
        m >>= 1;
    }
    idx_t x = n, y = k;
    while (y != 0) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return (seq_t)x;
}
