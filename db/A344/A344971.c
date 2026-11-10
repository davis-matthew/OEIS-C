/*
 * A344971  a(n) = A344875(n) / gcd(A011772(n), A344875(n)).
 * https://oeis.org/A344971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A344875(n) / gcd(A011772(n), A344875(n)); A011772 by
 *            search, A344875 multiplicative by trial division
 * Range:     u8 n <= 187, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 111 terms through n = 8666 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344971_A344875(uint64_t n) {
    seq_t ans = 1;
    uint64_t tmp = n;
    for (uint64_t p = 2; tmp > 1; p += (p == 2 ? 1 : 2)) {
        if (p * p > tmp) p = tmp;
        if (tmp % p) continue;
        unsigned e = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }
        seq_t term;
        if (p == 2) {
            term = 1;
            for (unsigned i = 0; i < e + 1; ++i) term *= 2;
            term -= 1;
        } else {
            term = 1;
            for (unsigned i = 0; i < e; ++i) term *= (seq_t)p;
            term -= 1;
        }
        ans *= term;
    }
    return ans;
}

static uint64_t A344971_A011772(uint64_t n) {
    for (uint64_t m = 1; m < 2 * n; ++m) {
        uint64_t x, y;
        if (m % 2 == 0) { x = m / 2; y = m + 1; }
        else { x = m; y = (m + 1) / 2; }
        x %= n; y %= n;
        if ((x * y) % n == 0) return m;
    }
    return 2 * n - 1;
}

static seq_t A344971_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t f = A344971_A344875((uint64_t)n);
    seq_t s = (seq_t)A344971_A011772((uint64_t)n);
    seq_t g = A344971_gcd(s, f);
    return f / g;
}
