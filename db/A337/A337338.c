/*
 * A337338  Numerator of (1+sigma(s)) / ((s+1)/2), where s is the square of n
 *          prime-shifted once (s = A003961(n)^2 = A003961(n^2)).
 * https://oeis.org/A337338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (1+sigma(s)) / gcd(1+sigma(s), (s+1)/2), s = A003961(n)^2
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9870 (2026-11-08)
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

static seq_t A337338_pw(seq_t b, unsigned e) {
    seq_t r = 1;
    while (e--) r *= b;
    return r;
}

seq_t a(idx_t n) {
    seq_t s_root = 1;     /* A003961(n) */
    seq_t sig = 1;        /* sigma(s) */
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            seq_t q = (seq_t)next_prime(p);
            s_root *= A337338_pw(q, e);
            sig *= (A337338_pw(q, 2 * e + 1) - 1) / (q - 1);
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime(m);
        s_root *= q;
        sig *= (q * q * q - 1) / (q - 1);
    }
    seq_t s = s_root * s_root;
    seq_t t = (s + 1) / 2;
    seq_t num = sig + 1;
    seq_t g = gcd(num, t);
    return num / g;
}
