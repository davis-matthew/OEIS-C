/*
 * A285703  a(n) = A000203(A064216(n)).
 * https://oeis.org/A285703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(A064216(n)); A064216(n) replaces each p^e || (2n-1)
 *            by prevprime(p)^e. Since prevprime is injective, sigma is the
 *            product over p^e || 2n-1 of (1+q+...+q^e), with q = prevprime(p)
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 6170 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A285703_is_prime(uint64_t x)
{
    if (x < 2) return 0;
    if (x < 4) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t A285703_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !A285703_is_prime(q)) --q;
    return q;
}

seq_t a(idx_t n)
{
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    seq_t result = 1;
    uint64_t p;

    for (p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t q = A285703_prevprime(p);
            seq_t term = 1;
            seq_t pw = 1;
            while (t % p == 0) {
                t /= p;
                pw *= (seq_t)q;
                term += pw;
            }
            result *= term;
        }
    }
    if (t > 1) {
        uint64_t q = A285703_prevprime(t);
        result *= ((seq_t)q + 1);
    }
    return result;
}
