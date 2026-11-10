/*
 * A354873  a(n) = A258851(n) / gcd(A056239(n), A258851(n)).
 * https://oeis.org/A354873
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A258851(n)/gcd(A056239(n), A258851(n)), where A258851(n)
 *            = n*Sum_{p^e||n} e*pi(p)/p and A056239(n) = Sum_{p^e||n} e*pi(p)
 * Range:     u8 n <= 95, u16 n <= 13055, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 9920 (2026-11-10)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

static seq_t A354873_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t deriv = 0;  /* A258851(n) */
    seq_t wsum = 0;   /* A056239(n) */
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        idx_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        seq_t pv = prime_pi(p);
        wsum += (seq_t)e * pv;
        deriv += (seq_t)e * pv * (seq_t)(n / p);
    }
    if (m > 1) {
        seq_t pv = prime_pi(m);
        wsum += pv;
        deriv += pv * (seq_t)(n / m);
    }
    seq_t g = A354873_gcd(wsum, deriv);
    if (g == 0) return deriv;
    return deriv / g;
}
