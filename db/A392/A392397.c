/*
 * A392397  Primes p that are congruent to 1 (mod 10) for which 5 |
 *          A001177(p).
 * https://oeis.org/A392397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p == 1 (mod 10) such that 5 | A001177(p);
 *            found by search, computing the entry point of p as a divisor of
 *            p-1 via Fibonacci doubling mod p
 * Range:     u8 n <= 12, u16 n <= 1361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9670 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A392397_fibmod(uint64_t k, uint64_t p) {
    /* returns F(k) mod p via fast doubling */
    uint64_t a = 0, b = 1 % p;
    int top = 63;
    while (top >= 0 && !((k >> top) & 1)) top--;
    for (int i = top; i >= 0; i--) {
        uint64_t t = (2 * b + p - a) % p;
        uint64_t c = (uint64_t)(((unsigned __int128)a * t) % p);
        uint64_t d = (uint64_t)(((unsigned __int128)a * a + (unsigned __int128)b * b) % p);
        if ((k >> i) & 1) {
            a = d;
            b = (c + d) % p;
        } else {
            a = c;
            b = d;
        }
    }
    return a;
}

static int A392397_isprime(uint64_t x) {
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t A392397_rank(uint64_t p) {
    uint64_t k = p - 1;
    uint64_t m = p - 1;
    for (uint64_t q = 2; q * q <= m; q++) {
        if (m % q == 0) {
            while (m % q == 0) m /= q;
            while (k % q == 0 && A392397_fibmod(k / q, p) == 0) k /= q;
        }
    }
    if (m > 1) {
        uint64_t q = m;
        while (k % q == 0 && A392397_fibmod(k / q, p) == 0) k /= q;
    }
    return k;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t p = 1;
    while (1) {
        p += 10;
        if (!A392397_isprime(p)) continue;
        if (A392397_rank(p) % 5 == 0) {
            count++;
            if (count == (uint64_t)n) return (seq_t)p;
        }
    }
}
