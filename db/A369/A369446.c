/*
 * A369446  Denominator of sigma(n) / A276086(n), where A276086 is the
 *          primorial base exp-function.
 * https://oeis.org/A369446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A276086(n) / gcd(sigma(n), A276086(n)), where A276086(n)
 *            = Product_k prime(k+1)^d_k and d_k are the primorial-base digits
 *            of n. Shared prime factors are cancelled one digit at a time, so
 *            the full A276086(n) is never formed
 * Range:     u8 n <= 20, u16 n <= 110, u32 n <= 625, u64 n <= 6892,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 89 terms through n = 9964 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* sigma(n) in native 64-bit arithmetic, by trial division */
static uint64_t A369446_sigma(uint64_t n)
{
    uint64_t res = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t term = 1, pk = 1;
            while (m % p == 0) {
                m /= p;
                pk *= p;
                term += pk;
            }
            res *= term;
        }
    }
    if (m > 1) res *= (m + 1);
    return res;
}

static uint64_t A369446_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; ++c) {
        bool ok = c >= 2;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) return c;
    }
}

seq_t a(idx_t n)
{
    uint64_t s = A369446_sigma((uint64_t)n);
    seq_t result = 1;
    uint64_t p = 2;
    uint64_t m = (uint64_t)n;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        /* cancel factors of p shared with sigma(n) */
        while (d > 0 && s % p == 0) {
            s /= p;
            --d;
        }
        for (uint64_t i = 0; i < d; ++i) result *= (seq_t)p;
        p = A369446_next_prime(p);
    }
    return result;
}
