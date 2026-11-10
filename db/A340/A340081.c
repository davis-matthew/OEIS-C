/*
 * A340081  a(n) = gcd(n-1, A003958(n)).
 * https://oeis.org/A340081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n-1, A003958(n)), where A003958 is multiplicative
 *            with A003958(p^e) = (p-1)^e; computed as gcd(n-1, A003958(n) mod
 *            (n-1)) to avoid overflow
 * Range:     u8 n <= 256, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 117 terms through n = 8175 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A340081_mulmod(idx_t a, idx_t b, idx_t m) {
    return (idx_t)(((unsigned __int128)a * b) % m);
}

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)1;
    idx_t m = n - 1;
    idx_t r = 1 % m;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t f = (p - 1) % m;
            while (temp % p == 0) {
                temp /= p;
                r = A340081_mulmod(r, f, m);
            }
        }
    }
    if (temp > 1) {
        r = A340081_mulmod(r, (temp - 1) % m, m);
    }
    idx_t x = m, y = r;
    while (y != 0) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return (seq_t)x;
}
