/*
 * A349826  Iterate x -> A349824(x) starting at n; if trajectory reaches a
 *          fixed point then that is a(n), if it ends in the loop (28,33) then
 *          a(n) = 28, otherwise a(n) = -1.
 * https://oeis.org/A349826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = limit of iterating x -> A349824(x) from n, where
 *            A349824(x) = bigomega(x)*sopfr(x) (0 for x<=1); a fixed point is
 *            returned, the loop (28,33) gives 28, otherwise -1 (found by
 *            direct iteration)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9423 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A349824_step(uint64_t m)
{
    if (m <= 1)
        return 0;
    uint64_t omega = 0, sum_pf = 0;
    while ((m & 1ULL) == 0) {
        omega++;
        sum_pf += 2;
        m >>= 1;
    }
    for (uint64_t d = 3; d <= (m / d); d += 2) {
        while (m % d == 0) {
            omega++;
            sum_pf += d;
            m /= d;
        }
    }
    if (m > 1) {
        omega++;
        sum_pf += m;
    }
    return omega * sum_pf;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    for (int i = 0; i < 10000; i++) {
        uint64_t y = A349824_step(x);
        if (y == x)
            return (seq_t)x;
        if ((x == 28 && y == 33) || (x == 33 && y == 28))
            return (seq_t)28;
        x = y;
    }
    return (seq_t)(0) - (seq_t)1;
}
