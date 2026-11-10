/*
 * A322036  a(n) = A322035(n) - A322034(n).
 * https://oeis.org/A322036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A322035(n) - A322034(n); with s = sum_{i=1..k}
 *            1/(p1*...*pi) (p1<=...<=pk the prime factors of n with
 *            multiplicity), a(n) = (n - S)/gcd(S,n) where S = sum_i
 *            n/(p1*...*pi). a(1)=1
 * Range:     u8 n <= 256, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 87 terms through n = 9855 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t A322036_gcd(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)1;
    const uint64_t orig = (uint64_t)n;
    uint64_t m = orig;
    uint64_t pref = 1;
    uint64_t sum = 0;

    while ((m & 1) == 0) {
        pref *= 2ULL;
        sum += orig / pref;
        m >>= 1;
    }
    for (uint64_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) {
            pref *= i;
            sum += orig / pref;
            m /= i;
        }
    }
    if (m > 1) {
        pref *= m;
        sum += orig / pref;
    }
    uint64_t g = A322036_gcd(sum, orig);
    return (seq_t)((orig - sum) / g);
}
