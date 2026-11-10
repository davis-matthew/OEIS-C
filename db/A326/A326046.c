/*
 * A326046  a(n) = gcd(n-A326039(n), A326040(n)-n).
 * https://oeis.org/A326046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n - A326039(n), A326040(n) - n), where A326039(n) is
 *            the largest square dividing sigma(n) and A326040(n) = sigma(n) -
 *            A326039(n)
 * Range:     u8 n <= 256, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9889 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A326046_sigma(uint64_t n)
{
    uint64_t sigma = 1;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t term = 1, power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                term += power;
            }
            sigma *= term;
        }
    }
    if (temp > 1)
        sigma *= (1 + temp);
    return sigma;
}

static uint64_t A326046_sqdiv(uint64_t x)
{
    uint64_t res = 1;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            for (int i = 0; i < e / 2; ++i)
                res *= p * p;
        }
    }
    return res;
}

static uint64_t A326046_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t sig = A326046_sigma(nn);
    uint64_t s = A326046_sqdiv(sig);
    uint64_t t = sig - s;
    uint64_t x = nn >= s ? nn - s : s - nn;
    uint64_t y = t >= nn ? t - nn : nn - t;
    return (seq_t)A326046_gcd(x, y);
}
