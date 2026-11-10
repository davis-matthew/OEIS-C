/*
 * A324645  a(n) = gcd(d(n), A276086(n)), where d(n) gives the number of
 *          divisors (A000005).
 * https://oeis.org/A324645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A000005(n), A276086(n)), computed with A276086(n)
 *            reduced mod d(n)
 * Range:     u8 n <= 30030, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 121 terms through n = 9691 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A324645_tau(uint64_t m)
{
    uint64_t r = 1, p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            r *= (e + 1);
        }
    }
    if (m > 1) r *= 2;
    return r;
}

static uint64_t A324645_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t d = A324645_tau(N);
    uint64_t r = 1 % d;
    uint64_t p = 2;
    uint64_t m = N;
    while (m > 0) {
        uint64_t digit = m % p;
        m /= p;
        uint64_t i;
        for (i = 0; i < digit; i++)
            r = (r * (p % d)) % d;
        /* next prime */
        do {
            p++;
            uint64_t q;
            int ok = 1;
            for (q = 2; q * q <= p; q++)
                if (p % q == 0) { ok = 0; break; }
            if (ok) break;
        } while (1);
    }
    return (seq_t)A324645_gcd(d, r);
}
