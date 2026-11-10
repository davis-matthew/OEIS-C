/*
 * A374104  a(n) = A075860(A075860(n) + 2) + 2.
 * https://oeis.org/A374104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A075860(A075860(n) + 2) + 2, where A075860(1)=0 and
 *            otherwise A075860(m) iterates x -> sum of distinct prime factors
 *            of x until a prime is reached
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 92 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t A374104_sopf(uint64_t x) {
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

static bool A374104_isprime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

static uint64_t A374104_A075860(uint64_t n) {
    if (n <= 1) return 0;
    uint64_t cur = n;
    while (true) {
        if (A374104_isprime(cur)) return cur;
        cur = A374104_sopf(cur);
    }
}

seq_t a(idx_t n) {
    uint64_t inner = A374104_A075860((uint64_t)n);
    uint64_t outer = A374104_A075860(inner + 2);
    return (seq_t)outer + 2;
}
