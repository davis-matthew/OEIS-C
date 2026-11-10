/*
 * A285704  a(n) = A285703(n) - n = A000203(A064216(n)) - n.
 * https://oeis.org/A285704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(A064216(n)) - n, where A064216(n) replaces each p^e
 *            in 2n-1 by prevprime(p)^e. Since prevprime is injective on
 *            primes, sigma is multiplicative over the factors: sigma =
 *            Product (1 + q + ... + q^e), q = prevprime(p)
 * Range:     i8 n <= 134, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 9146 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int a285704_is_prime(uint64_t x)
{
    if (x < 2) return 0;
    if (x < 4) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t a285704_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q >= 2 && !a285704_is_prime(q)) --q;
    return q;
}

static seq_t a285704_term(uint64_t p, int e)
{
    uint64_t q = a285704_prevprime(p);
    seq_t sum = 1, pw = 1;
    for (int i = 0; i < e; ++i) {
        pw *= (seq_t)q;
        sum += pw;
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    seq_t result = 1;

    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            result *= a285704_term(p, e);
        }
    }
    if (t > 1)
        result *= a285704_term(t, 1);

    return result - (seq_t)n;
}
