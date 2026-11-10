/*
 * A354871  a(n) = gcd(A056239(n), A258851(n)).
 * https://oeis.org/A354871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A056239(n), A258851(n)); A056239(n) = Sum_{p^e||n}
 *            e*pi(p), A258851(n) = Sum_{p^e||n} e*pi(p)*n/p. a(1) = 0
 * Range:     u8 n <= 1617, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9076 (2026-11-10)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

static seq_t A354871_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t s1 = 0;  /* A056239 */
    seq_t s2 = 0;  /* A258851 */
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        idx_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        seq_t pi = prime_pi(p);
        s1 += (seq_t)e * pi;
        s2 += (seq_t)e * pi * (seq_t)(n / p);
    }
    if (m > 1) {
        seq_t pi = prime_pi(m);
        s1 += pi;
        s2 += pi * (seq_t)(n / m);
    }
    return A354871_gcd(s1, s2);
}
