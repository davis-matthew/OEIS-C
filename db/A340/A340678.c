/*
 * A340678  a(n) = A008472(n) / gcd(A007947(n), A008472(n)).
 * https://oeis.org/A340678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008472(n) / gcd(A007947(n), A008472(n)), where A008472 =
 *            sum of distinct primes dividing n and A007947 = product of
 *            distinct primes dividing n
 * Range:     u8 n <= 513, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 9749 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A340678_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t prod = 1;
    seq_t sum = 0;

    if (m % 2 == 0) {
        prod *= 2;
        sum += 2;
        while (m % 2 == 0) m /= 2;
    }
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            prod *= (seq_t)p;
            sum += (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        prod *= (seq_t)m;
        sum += (seq_t)m;
    }

    seq_t g = A340678_gcd(prod, sum);
    return sum / g;
}
