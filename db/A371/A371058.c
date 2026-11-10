/*
 * A371058  Numbers k divisible by A075860(k).
 * https://oeis.org/A371058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 2 such that A075860(k) divides k; found by a
 *            search over k, where A075860(k) is the prime reached by
 *            iterating x -> sum of distinct prime factors of x
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t A371058_sopf(uint64_t x) {
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

static bool A371058_isprime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

static uint64_t A371058_fixed(uint64_t k) {
    uint64_t cur = k;
    while (!A371058_isprime(cur)) cur = A371058_sopf(cur);
    return cur;
}

seq_t a(idx_t n) {
    uint64_t k = 1;
    idx_t cnt = 0;
    while (cnt < n) {
        k++;
        uint64_t f = A371058_fixed(k);
        if (k % f == 0) cnt++;
    }
    return (seq_t)k;
}
