/*
 * A381953  Numbers k such that A381952(k) = 2.
 * https://oeis.org/A381953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that gcd(k, A051903(k)) = 2; found by
 *            scanning k upward and counting
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9253 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A381953_maxexp(uint64_t k)
{
    uint64_t best = 0;
    for (uint64_t p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            uint64_t e = 0;
            while (k % p == 0) { k /= p; e++; }
            if (e > best) best = e;
        }
    }
    if (k > 1 && best < 1) best = 1;
    return best;
}

static uint64_t A381953_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k += 2;
        uint64_t e = A381953_maxexp(k);
        if ((e & 1) == 0 && e > 0 && A381953_gcd(k, e) == 2)
            count++;
    }
    return (seq_t)k;
}
