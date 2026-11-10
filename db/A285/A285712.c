/*
 * A285712  a(1) = 0, and for n > 1, if n = 3k-1, then a(n) = k, otherwise
 *          a(n) = (A064216(n)+1)/2.
 * https://oeis.org/A285712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = (n+1)/3 if n == 2 (mod 3), otherwise
 *            a(n) = (A064216(n)+1)/2
 * Range:     u8 n <= 260, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 111 terms through n = 8135 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A285712_is_prime(uint64_t x)
{
    if (x < 2) return 0;
    if (x == 2) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t A285712_prevprime(uint64_t p)
{
    for (uint64_t q = p - 1; q >= 2; --q)
        if (A285712_is_prime(q))
            return q;
    return 2;
}

static seq_t A285712_A064216(idx_t n)
{
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    if (t <= 1ULL)
        return (seq_t)1;
    uint64_t temp = t;
    seq_t result = 1;
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            uint64_t prev = A285712_prevprime(p);
            for (int i = 0; i < exp; ++i)
                result *= (seq_t)prev;
        }
    }
    if (temp > 1ULL) {
        result *= (seq_t)A285712_prevprime(temp);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    if (n % 3 == 2)
        return (seq_t)((n + 1) / 3);
    seq_t v = A285712_A064216(n);
    return (v + 1) / 2;
}
