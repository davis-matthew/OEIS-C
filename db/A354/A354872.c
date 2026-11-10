/*
 * A354872  a(n) = A056239(n) / gcd(A056239(n), A258851(n)).
 * https://oeis.org/A354872
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A056239(n)/gcd(A056239(n), A258851(n)), where A056239(n)
 *            = Sum_{p^e||n} e*pi(p) and A258851(n) = n*Sum_{p^e||n}
 *            e*pi(p)/p; A258851 is only computed mod A056239(n), which is
 *            enough for the gcd
 * Range:     u8 n <= 3225, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9094 (2026-11-10)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n) {
    uint64_t ps[24];
    seq_t w[24];   /* e * pi(p) */
    int cnt = 0;
    seq_t A = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        idx_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        seq_t t = (seq_t)e * (seq_t)prime_pi(p);
        ps[cnt] = p; w[cnt] = t; ++cnt;
        A += t;
    }
    if (m > 1) {
        seq_t t = (seq_t)prime_pi(m);
        ps[cnt] = m; w[cnt] = t; ++cnt;
        A += t;
    }
    if (A == 0) return 0;
    seq_t B = 0; /* A258851(n) mod A */
    for (int i = 0; i < cnt; ++i) {
        seq_t q = (seq_t)(n / ps[i]) % A;
        B = (B + (w[i] % A) * q) % A;
    }
    seq_t x = A, y = B;
    while (y != 0) {
        seq_t r = x % y;
        x = y;
        y = r;
    }
    return A / x;
}
