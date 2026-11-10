/*
 * A349825  Number of steps when x -> A349824(x) is iterated starting at n
 *          needed to reach fixed point or 28, or -1 if trajectory increases
 *          for ever or ends in a nontrivial loop other than (28,33).
 * https://oeis.org/A349825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> A349824(x) from n to reach a
 *            fixed point (f(x)=x) or 28; -1 if the orbit enters another loop
 *            or grows (iteration with cycle detection)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 8655 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A349825_f(uint64_t m)
{
    if (m <= 1) return 0;
    uint64_t omega = 0, sum = 0;
    while ((m & 1ULL) == 0) { omega++; sum += 2; m >>= 1; }
    for (uint64_t d = 3; d <= (m / d); d += 2) {
        while (m % d == 0) { omega++; sum += d; m /= d; }
    }
    if (m > 1) { omega++; sum += m; }
    return omega * sum;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t saved = x;
    uint64_t power = 1, lam = 0;
    uint64_t steps = 0;
    while (steps < 200000ULL) {
        if (x == 28) return (seq_t)steps;
        uint64_t y = A349825_f(x);
        if (y == x) return (seq_t)steps;
        x = y;
        steps++;
        if (x == saved) return (seq_t)0 - (seq_t)1;
        lam++;
        if (lam == power) { saved = x; power <<= 1; lam = 0; }
        if (x > ((uint64_t)1 << 50)) return (seq_t)0 - (seq_t)1;
    }
    return (seq_t)0 - (seq_t)1;
}
