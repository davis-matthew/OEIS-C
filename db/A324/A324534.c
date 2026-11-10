/*
 * A324534  The smallest common prime factor of sigma(n) and A276086(n), or 1
 *          if no such prime exists.
 * https://oeis.org/A324534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A020639(gcd(sigma(n), A276086(n))); the primes dividing
 *            A276086(n) are those prime(k+1) with nonzero digit d_k in the
 *            primorial base of n, so a(n) is the least such prime dividing
 *            sigma(n), else 1
 * Range:     u8 n <= 30030, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 121 terms through n = 8441 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A324534_sigma(uint64_t m)
{
    uint64_t result = 1;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) result *= (m + 1);
    return result;
}

static bool A324534_isprime(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; d++)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t sg = A324534_sigma(m);
    uint64_t p = 2;
    while (m > 0) {
        uint64_t digit = m % p;
        m /= p;
        if (digit != 0 && sg % p == 0)
            return (seq_t)p;
        p++;
        while (!A324534_isprime(p)) p++;
    }
    return (seq_t)1;
}
