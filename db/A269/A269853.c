/*
 * A269853  Permutation of natural numbers: a(1) = 1, a(2n) = 2*a(n), a(2n+1)
 *          = 1 + 2*a(A064216(n)).
 * https://oeis.org/A269853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2n) = 2*a(n), a(2n+1) = 1 + 2*a(A064216(n)), where
 *            A064216(n) = A064989(2n-1) replaces each odd prime factor p^e by
 *            prevprime(p)^e
 * Range:     u8 n <= 80, u16 n <= 944, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 99 terms through n = 7836 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A269853_isp(uint64_t x)
{
    if (x < 2) return 0;
    if (x < 4) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t A269853_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q >= 2 && !A269853_isp(q)) --q;
    return q;
}

static uint64_t A269853_A064216(uint64_t n)
{
    uint64_t t = 2 * n - 1;
    uint64_t result = 1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t prev = A269853_prevprime(p);
            while (t % p == 0) {
                t /= p;
                result *= prev;
            }
        }
    }
    if (t > 1) result *= A269853_prevprime(t);
    return result;
}

static seq_t A269853_rec(uint64_t n)
{
    if (n <= 1) return (seq_t)1;
    if ((n & 1) == 0) return (seq_t)2 * A269853_rec(n / 2);
    uint64_t m = (n - 1) / 2;
    return (seq_t)1 + (seq_t)2 * A269853_rec(A269853_A064216(m));
}

seq_t a(idx_t n)
{
    return A269853_rec((uint64_t)n);
}
