/*
 * A290534  Denominator of 2*n*(2*n+1) B_{2*n}, where B_n are the Bernoulli
 *          numbers.
 * https://oeis.org/A290534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 125, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 80 terms through n = 462 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* By von Staudt-Clausen the denominator of B_{2n} (n >= 1) is the product of
   the primes p with (p - 1) | 2n, each to the first power; multiplying by
   2n(2n+1) cancels exactly those p that divide 2n(2n+1). So a(n) is the
   product of the primes p = d + 1, d | 2n, with p dividing neither 2n nor
   2n + 1; a(0) = 1 (0 * B_0 = 0). (The original sieved to a fixed bound and
   overran it.) */

#include <stdint.h>
#include "../utils/is_prime_u64.h"

static seq_t factor(uint64_t d, uint64_t m)          /* p = d + 1 if it counts, else 1 */
{
    uint64_t p = d + 1;
    if (!is_prime_u64(p) || m % p == 0 || (m + 1) % p == 0) return (seq_t)1;
    return (seq_t)p;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    uint64_t m = 2 * (uint64_t)n;
    seq_t r = 1;
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d) continue;
        r = r * factor(d, m);
        if (d != m / d) r = r * factor(m / d, m);
    }
    return r;
}
