/*
 * A360591  Primes in A360464.
 * https://oeis.org/A360591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = n-th prime in A360464, where A360464(k) = A360464(k-1) +
 *            A360464(k-2) - A360464(k-3) + gcd(A360464(k-1), A360464(k-3)),
 *            A360464(1) = A360464(2) = A360464(3) = 1; search over k <= 457
 *            (finite sequence, 103 terms)
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A360591_gcd(uint64_t x, uint64_t y)
{
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static bool A360591_isprime(uint64_t m)
{
    uint64_t d;
    if (m < 2) return false;
    if (m < 4) return true;
    if (m % 2 == 0) return false;
    for (d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t x3 = 1, x2 = 1, x1 = 1; /* A360464(k-3), A360464(k-2), A360464(k-1) */
    idx_t count = 0;
    idx_t k;
    if (n < 1 || n > 103) return (seq_t)0;
    /* A360464 is strictly increasing from k = 4 on, so primes are distinct */
    for (k = 4; k <= 457; ++k) {
        uint64_t nxt = x1 + x2 - x3 + A360591_gcd(x1, x3);
        x3 = x2;
        x2 = x1;
        x1 = nxt;
        if (A360591_isprime(nxt)) {
            ++count;
            if (count == n) return (seq_t)nxt;
        }
    }
    return (seq_t)0;
}
