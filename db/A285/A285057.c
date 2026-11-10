/*
 * A285057  a(n) = lcm(n, A001177(n)).
 * https://oeis.org/A285057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, A001177(n)); A001177(n) found by iterating
 *            Fibonacci numbers mod n
 * Range:     u8 n <= 18, u16 n <= 270, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 76 terms through n = 4736 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A285057_entry(uint64_t m) {
    if (m == 1) return 1;
    uint64_t f0 = 0, f1 = 1, k = 1;
    while (f1 != 0) {
        uint64_t f2 = f0 + f1;
        if (f2 >= m) f2 -= m;
        f0 = f1;
        f1 = f2;
        k++;
    }
    return k;
}

static uint64_t A285057_gcd(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t k = A285057_entry(m);
    uint64_t g = A285057_gcd(m, k);
    return (seq_t)(m / g) * (seq_t)k;
}
