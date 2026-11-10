/*
 * A373249  a(n) = A181819(n) / gcd(n, A181819(n)), where A181819(n) is the
 *          prime shadow of n.
 * https://oeis.org/A373249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A181819(n) / gcd(n, A181819(n)), where A181819(n) =
 *            Product prime(e_i) for n = Product prime_i^e_i
 * Range:     u8 n <= 763, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8801 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint32_t A373249_nth_prime(uint64_t k)
{
    uint32_t cnt = 0;
    uint32_t c = 1;
    while (cnt < k) {
        ++c;
        bool ok = true;
        for (uint32_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) ++cnt;
    }
    return c;
}

static seq_t A373249_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t s = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        s = s * (seq_t)A373249_nth_prime(exps[i]);

    seq_t g = A373249_gcd((seq_t)n, s);
    return s / g;
}
