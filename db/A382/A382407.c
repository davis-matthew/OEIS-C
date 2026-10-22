/*
 * A382407  a(n) is the number of partitions n = x + y + z of positive
 *          integers such that x*y + y*z + x*z is a perfect square.
 * https://oeis.org/A382407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered integer triples (x,y,z) with * x ≤ y
 *            ≤ z, x + y + z = n, and x*y + y*z + x*z is a perfect square
 * Range:     u8 n <= 1539, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for uint64_t values – works in O(log n) */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n == 0)   return 0;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x <= n / 3; ++x) {
        for (idx_t y = x; y <= (n - x) / 2; ++y) {
            idx_t z = n - x - y;
            uint64_t t = (uint64_t)x * y + (uint64_t)y * z + (uint64_t)x * z;
            uint64_t s = isqrt_u64(t);
            if (s * s == t) {
                ++count;
            }
        }
    }
    return count;
}
