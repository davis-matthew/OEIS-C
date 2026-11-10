/*
 * A327858  Greatest common divisor of the arithmetic derivative and the
 *          primorial base exp-function: a(n) = gcd(A003415(n), A276086(n)).
 * https://oeis.org/A327858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = gcd(A003415(n), A276086(n)); a(0)=1, a(1)=2
 * Range:     u8 n <= 19, u16 n <= 85, u32 n <= 3479, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9896 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A327858_nextprime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; ++c) {
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok && c >= 2) return c;
    }
}

static seq_t A327858_gcd(seq_t x, seq_t y)
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
    /* A276086(n): digits of n in primorial base are exponents of successive primes */
    seq_t ex = 1;
    {
        idx_t rem = n;
        uint64_t p = 2;
        while (rem > 0) {
            idx_t d = rem % p;
            rem /= p;
            for (idx_t i = 0; i < d; ++i) ex *= (seq_t)p;
            p = A327858_nextprime(p);
        }
    }

    /* A003415(n): arithmetic derivative */
    seq_t der = 0;
    if (n > 1) {
        idx_t rem = n;
        for (idx_t p = 2; p * p <= rem; ++p) {
            int e = 0;
            while (rem % p == 0) { rem /= p; ++e; }
            if (e > 0) der += (seq_t)(n / p) * (seq_t)e;
        }
        if (rem > 1) der += (seq_t)(n / rem);
    }

    return A327858_gcd(der, ex);
}
