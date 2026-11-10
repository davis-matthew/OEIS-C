/*
 * A285705  a(n) = 2*n - A285703(n) = 2*n - A000203(A064216(n)).
 * https://oeis.org/A285705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = 2*n - sigma(A064216(n)); A064216(n) replaces each p^e ||
 *            2n-1 by prevprime(p)^e; sigma is multiplicative
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 8803 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A285705_isp(uint64_t x)
{
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t A285705_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !A285705_isp(q)) --q;
    return q;
}

seq_t a(idx_t n)
{
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    seq_t sig = 1;
    uint64_t p;

    for (p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t q = A285705_prevprime(p);
            seq_t term = 1, pw = 1;
            while (t % p == 0) {
                t /= p;
                pw *= (seq_t)q;
                term += pw;
            }
            sig *= term;
        }
    }
    if (t > 1) {
        uint64_t q = A285705_prevprime(t);
        sig *= ((seq_t)q + 1);
    }
    return (seq_t)2 * (seq_t)n - sig;
}
