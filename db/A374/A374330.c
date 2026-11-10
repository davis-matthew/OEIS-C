/*
 * A374330  a(n) is the number of numbers k <= prime(n)^2 such that A075860(k)
 *          = prime(n).
 * https://oeis.org/A374330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k <= prime(n)^2 : A075860(k) = prime(n) }; computed by
 *            direct search, iterating x -> sum of distinct prime factors
 *            until a prime is reached
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t A374330_sopf(uint64_t x) {
    uint64_t sum = 0;
    if ((x & 1ULL) == 0) {
        sum += 2;
        while ((x & 1ULL) == 0) x >>= 1;
    }
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) {
            sum += d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) sum += x;
    return sum;
}

static bool A374330_isprime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* A075860 for k >= 2 */
static uint64_t A374330_fix(uint64_t k) {
    uint64_t cur = k;
    while (!A374330_isprime(cur)) cur = A374330_sopf(cur);
    return cur;
}

seq_t a(idx_t n) {
    uint64_t cnt = 0, p = 1;
    uint64_t idx = 0;
    while (idx < (uint64_t)n) {
        p++;
        if (A374330_isprime(p)) idx++;
    }
    uint64_t lim = p * p;
    for (uint64_t k = 2; k <= lim; k++) {
        if (A374330_fix(k) == p) cnt++;
    }
    return (seq_t)cnt;
}
