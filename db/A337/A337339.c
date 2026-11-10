/*
 * A337339  Denominator of (1+sigma(s)) / ((s+1)/2), where s is the square of
 *          n prime-shifted once (s = A003961(n)^2 = A003961(n^2)).
 * https://oeis.org/A337339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = ((s+1)/2) / gcd((s+1)/2, 1+sigma(s)), where s =
 *            A003961(n)^2
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1535, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 74 terms through n = 7848 (2026-11-08)
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

static seq_t A337339_geo(seq_t q, idx_t k) {
    /* 1 + q + ... + q^k */
    seq_t result = 1;
    seq_t power = 1;
    for (idx_t i = 0; i < k; i++) {
        power = power * q;
        result = result + power;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t ap = 1;
    seq_t sig = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t q = (seq_t)next_prime((uint64_t)p);
            for (idx_t i = 0; i < e; i++) ap = ap * q;
            sig = sig * A337339_geo(q, 2 * e);
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime((uint64_t)m);
        ap = ap * q;
        sig = sig * (1 + q + q * q);
    }
    seq_t s = ap * ap;
    seq_t t = (s + 1) / 2;
    seq_t g = gcd(t, sig + 1);
    return t / g;
}
