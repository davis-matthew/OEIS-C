/*
 * A373366  a(n) = gcd(A064097(n), A083345(n)).
 * https://oeis.org/A373366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A064097(n), A083345(n)); A064097 is completely
 *            additive with A064097(p) = 1 + A064097(p-1), and A083345(n) =
 *            n'/gcd(n,n') with n' the arithmetic derivative
 * Range:     u8 n <= 3963, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9296 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t A064097_f(uint64_t m)
{
    if (m <= 1) return (seq_t)0;
    seq_t sum = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            /* p is prime; p < m here, composite or prime power */
            sum += (seq_t)e * ((seq_t)1 + A064097_f(p - 1));
        }
    }
    if (t > 1) {
        if (t == m) return (seq_t)1 + A064097_f(m - 1);  /* m prime */
        sum += (seq_t)1 + A064097_f(t - 1);
    }
    return sum;
}

static seq_t A083345_g(idx_t n)
{
    if (n == 1) return (seq_t)0;
    uint64_t t = (uint64_t)n;
    seq_t ns = (seq_t)n;
    seq_t d = 0;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            d += (seq_t)e * (ns / (seq_t)p);
        }
    }
    if (t > 1) d += ns / (seq_t)t;
    seq_t g = gcd(ns, d);
    return d / g;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t x = A064097_f((uint64_t)n);
    seq_t y = A083345_g(n);
    return gcd(x, y);
}
