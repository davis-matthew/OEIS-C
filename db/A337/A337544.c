/*
 * A337544  a(n) = 2*phi(A003961(n)) - A003961(n).
 * https://oeis.org/A337544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*phi(A003961(n)) - A003961(n); with q = nextprime(p),
 *            A003961(p^e) = q^e and phi(A003961(p^e)) = (q-1)*q^(e-1), both
 *            multiplicative
 * Range:     i8 n <= 63, i16 n <= 2047, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 101 terms through n = 9699 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static void A337544_apply(uint64_t p, uint64_t e, seq_t *s, seq_t *t) {
    uint64_t q = next_prime(p);
    seq_t qs = (seq_t)q;
    *s *= qs;
    *t *= (seq_t)(q - 1);
    for (uint64_t i = 1; i < e; i++) {
        *s *= qs;
        *t *= qs;
    }
}

seq_t a(idx_t n) {
    seq_t s = 1, t = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            A337544_apply(p, e, &s, &t);
        }
    }
    if (m > 1) {
        A337544_apply(m, 1, &s, &t);
    }
    return 2 * t - s;
}
