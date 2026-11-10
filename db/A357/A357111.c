/*
 * A357111  For n >= 1, a(n) = n / A076775(n).
 * https://oeis.org/A357111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n / gcd(n, D(n)), where D(n) is the binary representation
 *            of n read as a decimal number (A076775)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9111 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A357111_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint64_t m = (uint64_t)n;
    int msb = 63 - __builtin_clzll(m);
    /* D mod n, computed bit by bit; gcd(n, D) = gcd(n, D mod n) */
    uint64_t r = 0;
    for (int i = msb; i >= 0; --i) {
        r = (r * 10 + ((m >> i) & 1)) % m;
    }
    uint64_t g = A357111_gcd(m, r);
    return (seq_t)(m / g);
}
