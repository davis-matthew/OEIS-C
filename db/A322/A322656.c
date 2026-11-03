/*
 * A322656  Denominator of (Sum_{d|n} sigma(d)) / sigma(n).
 * https://oeis.org/A322656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), sum_{d|n} sigma(d))
 * Range:     u8 n <= 59, u16 n <= 19999, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 85 terms through n = 9863 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Integer square‑root (floor) for uint64_t, no overflow. */
static uint64_t u64_sqrt(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = ((uint64_t)1) << 62;              /* largest power of 4 <= 2^64 */
    while (bit) {
        uint64_t tmp = r | bit;
        if (tmp != 0 && tmp <= x / tmp)
            r = tmp;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* sigma(n) */
    seq_t sigma_n = sigma_of(n);

    /* sum_{d|n} sigma(d) by enumerating divisors up to sqrt(n) */
    uint64_t sqrt_n = u64_sqrt((uint64_t)n);
    seq_t sum_sigma = 0;

    for (uint64_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            seq_t sigma_i = sigma_of(i);
            sum_sigma += sigma_i;

            uint64_t j = n / i;
            if (j != i)            /* avoid double counting perfect square root */
                sum_sigma += sigma_of(j);
        }
    }

    /* denominator = sigma_n / gcd(sigma_n, sum_sigma) */
    seq_t g = gcd(sigma_n, sum_sigma);
    return sigma_n / g;
}
