/*
 * A349827  Iterate x -> A349824(x) starting at n; a(n) is the greatest term
 *          in the trajectory, or -1 if the trajectory increases for ever.
 * https://oeis.org/A349827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max of trajectory under x -> A349824(x) starting at n
 *            (iterated until a fixed point or the loop (28,33) is reached);
 *            -1 if unbounded. A349824(x) = bigomega(x)*sopfr(x), A349824(0)=0
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9076 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A349824_step(uint64_t m)
{
    if (m <= 1) return m == 1 ? 1 : 0;
    uint64_t omega = 0, sum = 0;
    while ((m & 1ULL) == 0) { omega++; sum += 2; m >>= 1; }
    for (uint64_t d = 3; d <= m / d; d += 2) {
        while (m % d == 0) { omega++; sum += d; m /= d; }
    }
    if (m > 1) { omega++; sum += m; }
    return omega * sum;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t mx = x;
    for (int steps = 0; steps < 100000; steps++) {
        uint64_t y = A349824_step(x);
        if (y > mx) mx = y;
        if (y == x) return (seq_t)mx;
        if (y == 28 || y == 33) {
            if (33 > mx) mx = 33;
            return (seq_t)mx;
        }
        x = y;
    }
    return (seq_t)(-1);
}
