/*
 * A274987  Primes p such that A274601(p) is a prime.
 * https://oeis.org/A274987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th prime p such that A274601(p) =
 *            2*3^floor(log_3(2p-1)) - p is prime; found by a incremental
 *            search over primes
 * Range:     u8 n <= 28, u16 n <= 1709, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9999 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A274987_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (uint64_t d = 5; d * d <= x; d += 6)
        if (x % d == 0 || x % (d + 2) == 0) return false;
    return true;
}

static uint64_t A274987_a274601(uint64_t n)
{
    uint64_t m = n * 2 - 1;
    uint64_t p3 = 1;
    while (p3 <= m / 3) p3 *= 3;
    return p3 * 2 - n;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 2;

    if (n == 0) return (seq_t)0;
    while (cnt < (size_t)n) {
        uint64_t p = next;
        next = (p == 2) ? 3 : p + 2;
        if (!A274987_isp(p)) continue;
        if (!A274987_isp(A274987_a274601(p))) continue;
        if (cnt == cap) {
            cap = cap ? cap * 2 : 1024;
            terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
        }
        terms[cnt++] = p;
    }
    return (seq_t)terms[n - 1];
}
