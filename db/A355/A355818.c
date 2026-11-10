/*
 * A355818  Greatest common divisor of n, sigma(n) and A276086(n), where
 *          A276086 is primorial base exp-function.
 * https://oeis.org/A355818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, sigma(n), A276086(n)); computed as prod over
 *            primes p_k of p_k^min(v_p(gcd(n,sigma(n))), d_k), where d_k are
 *            the primorial-base digits of n
 * Range:     u8 n <= 4094, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9770 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t A355818_sigma(uint64_t m)
{
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) result *= ((seq_t)m + 1);
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t gs = gcd((seq_t)n, A355818_sigma((uint64_t)n));
    uint64_t g = (uint64_t)gs;
    if (g <= 1) return 1;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    uint64_t p = 2;
    while (m > 0 && g > 1) {
        uint64_t d = m % p;
        m /= p;
        if (d > 0 && g % p == 0) {
            uint64_t e = 0;
            while (g % p == 0) { g /= p; e++; }
            if (e > d) e = d;
            for (uint64_t t = 0; t < e; ++t) result *= (seq_t)p;
        }
        /* next prime */
        uint64_t c = (p == 2) ? 3 : p + 2;
        for (;; c += 2) {
            bool ok = true;
            for (uint64_t q = 3; q * q <= c; q += 2) {
                if (c % q == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        p = c;
    }
    return result;
}
