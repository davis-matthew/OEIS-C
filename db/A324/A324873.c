/*
 * A324873  a(n) = gcd(n, A060968(n)).
 * https://oeis.org/A324873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A060968(n)), where A060968 is multiplicative:
 *            A060968(2) = 2, A060968(2^e) = 2^(e+1) for e > 1, A060968(p^e) =
 *            (p-1)*p^(e-1) for p == 1 (mod 4), A060968(p^e) = (p+1)*p^(e-1)
 *            for p == 3 (mod 4)
 * Range:     u8 n <= 255, u16 n <= 65535, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9987 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t A324873_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A324873_A060968(uint64_t n)
{
    uint64_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            uint64_t part;
            if (p == 2) {
                part = 2;
                if (e > 1) {
                    part = 1;
                    for (uint64_t j = 0; j < e + 1; ++j) part *= 2;
                }
            } else {
                uint64_t pp = 1;
                for (uint64_t j = 1; j < e; ++j) pp *= p;
                part = ((p & 3ULL) == 1ULL) ? (p - 1) * pp : (p + 1) * pp;
            }
            result *= part;
        }
    }
    if (m > 1) {
        if (m == 2) result *= 2;
        else if ((m & 3ULL) == 1ULL) result *= (m - 1);
        else result *= (m + 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t v = A324873_A060968((uint64_t)n);
    return (seq_t)A324873_gcd((uint64_t)n, v);
}
