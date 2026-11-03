/*
 * A318505  Sum of divisors of n, up to, but not including the second largest
 *          of them A032742(n); a(1) = 0 by convention.
 * https://oeis.org/A318505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(n) – n – A032742(n) = sum of all divisors of n minus n
 *            and minus its second largest divisor (which equals n /
 *            smallest_prime_factor(n)). a(1) = 0 by convention
 * Range:     u8 n <= 179, u16 n <= 27719, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9506 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

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

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* sum of all divisors of n */
    seq_t sigma = sigma_of(n);

    /* smallest prime factor of n (for n > 1) */
    uint64_t spf = smallest_prime_factor(n);

    /* second largest divisor = n / spf */
    uint64_t second_val = n / spf;
    seq_t second = (seq_t)second_val;

    /* subtract n and the second largest divisor */
    sigma -= (seq_t)n;
    sigma -= second;

    return sigma;
}
