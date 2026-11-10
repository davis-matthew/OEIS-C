/*
 * A308194  Number of steps to reach 5 when iterating x -> A063655(x) starting
 *          at x=n.
 * https://oeis.org/A308194
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(5) = 0; a(n) = 1 + a(A063655(n)) for n > 5, where A063655(n) =
 *            Min_{d|n} (d + n/d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 9562 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A308194_semiperim(uint64_t n)
{
    uint64_t s = 0;
    while ((s + 1) * (s + 1) <= n) s++;
    for (uint64_t d = s; d >= 1; --d) {
        if (n % d == 0) return d + n / d;
    }
    return n + 1;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    unsigned long long steps = 0;
    while (x > 5) {
        x = A308194_semiperim(x);
        steps++;
    }
    return (seq_t)steps;
}
