/*
 * A243502  Permutation of even numbers: a(n) = 2 * A064216(n).
 * https://oeis.org/A243502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A064216(n), where A064216(n) = A064989(2n-1) replaces
 *            each prime p in the factorization of 2n-1 by prevprime(p)
 * Range:     u8 n <= 68, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 84 terms through n = 9826 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A243502_isprime(uint64_t x)
{
    if (x < 2) return 0;
    if (x < 4) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t A243502_prevprime(uint64_t p)
{
    for (uint64_t q = p - 1; q >= 2; --q)
        if (A243502_isprime(q)) return q;
    return 2;
}

seq_t a(idx_t n)
{
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    seq_t result = 1;
    uint64_t temp = t;
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t prev = A243502_prevprime(p);
            while (temp % p == 0) {
                temp /= p;
                result *= (seq_t)prev;
            }
        }
    }
    if (temp > 1ULL)
        result *= (seq_t)A243502_prevprime(temp);
    return result * 2;
}
