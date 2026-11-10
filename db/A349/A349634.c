/*
 * A349634  Denominators of the Möbius transform of ratio A003961(n)/n.
 * https://oeis.org/A349634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{d|n} mu(n/d)*A003961(d)/d;
 *            multiplicative, g(p^e) = q^(e-1)*(q-p)/p^e with q =
 *            nextprime(p), fractions reduced across primes
 * Range:     u8 n <= 255, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 100 terms through n = 7736 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static uint64_t A349634_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void A349634_cancel(uint64_t *den, uint64_t x) {
    uint64_t g = A349634_gcd(x, *den);
    *den /= g;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t den = m;
    uint64_t ps[20], es[20];
    int cnt = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            ps[cnt] = p; es[cnt] = e; cnt++;
        }
    }
    if (m > 1) { ps[cnt] = m; es[cnt] = 1; cnt++; }
    for (int i = 0; i < cnt; i++) {
        uint64_t p = ps[i];
        uint64_t q = next_prime(p);
        A349634_cancel(&den, q - p);
        for (uint64_t k = 1; k < es[i]; k++) A349634_cancel(&den, q);
    }
    return (seq_t)den;
}
