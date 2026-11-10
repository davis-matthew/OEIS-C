/*
 * A336839  Denominator of the arithmetic mean of the divisors of A003961(n).
 * https://oeis.org/A336839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = d(n)/gcd(d(n), A003973(n)), where d = A000005 and
 *            A003973(n) = Product_{p^e || n} (q^(e+1)-1)/(q-1), q =
 *            nextprime(p); sigma is taken mod d(n)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9792 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static uint64_t A336839_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t ps[64];
    unsigned es[64];
    int cnt = 0;
    uint64_t d = 1;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            ps[cnt] = p; es[cnt] = e; cnt++;
            d *= (e + 1);
        }
    }
    if (m > 1) { ps[cnt] = m; es[cnt] = 1; cnt++; d *= 2; }

    uint64_t s = 1 % d;
    for (int i = 0; i < cnt; i++) {
        uint64_t q = next_prime(ps[i]) % d;
        uint64_t term = 1 % d, pw = 1 % d;
        for (unsigned j = 0; j < es[i]; j++) {
            pw = (pw * q) % d;
            term = (term + pw) % d;
        }
        s = (s * term) % d;
    }
    uint64_t g = A336839_gcd(d, s);   /* gcd(d, 0) = d */
    return (seq_t)(d / g);
}
