/*
 * A332559  a(n) = n + A332558(n) + 1.
 * https://oeis.org/A332559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A332558(n) + 1, where A332558(n) is the smallest k
 *            such that n*(n+1)*...*(n+k) is divisible by n+k+1 (found by
 *            search)
 * Range:     u8 n <= 245, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9797 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A332559_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A332559_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static uint64_t A332559_k(uint64_t n)
{
    for (uint64_t k = 0;; ++k) {
        uint64_t M = n + k + 1;
        /* A prime M > n+k can never divide the product. */
        if (A332559_isprime(M)) continue;
        uint64_t r = M;
        for (uint64_t i = n; i <= n + k; ++i) {
            r /= A332559_gcd(r, i);
            if (r == 1) return k;
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t k = A332559_k((uint64_t)n);
    return (seq_t)n + (seq_t)k + 1;
}
