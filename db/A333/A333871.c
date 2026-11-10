/*
 * A333871  Sum of the iterated absolute Möbius divisor function (A173557).
 * https://oeis.org/A333871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n) + a(f(n)), a(1) = 0, where f = A173557, f(n) =
 *            Product_{p|n, p prime} (p-1)
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9998 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A333871_f(uint64_t m)
{
    uint64_t result = 1;
    if (m <= 1) return 1;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            result *= (p - 1);
            do { m /= p; } while (m % p == 0);
        }
    }
    if (m > 1) result *= (m - 1);
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t m = (uint64_t)n;
    while (m > 1) {
        m = A333871_f(m);
        sum += (seq_t)m;
    }
    return sum;
}
