/*
 * A346468  a(n) = (n-1) / A346467(n).
 * https://oeis.org/A346468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) / A346467(n), where A346467(n) = lcm{ d | n-1 : d+1
 *            prime }, a(1) = 0
 * Range:     u8 n <= 257, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 106 terms through n = 9454 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A346468_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t p = 3; p * p <= x; p += 2)
        if (x % p == 0) return false;
    return true;
}

static uint64_t A346468_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t m = (uint64_t)(n - 1);
    uint64_t L = 1;
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d != 0) continue;
        uint64_t e = m / d;
        if (A346468_isprime(d + 1))
            L = L / A346468_gcd(L, d) * d;
        if (e != d && A346468_isprime(e + 1))
            L = L / A346468_gcd(L, e) * e;
    }
    return (seq_t)(m / L);
}
