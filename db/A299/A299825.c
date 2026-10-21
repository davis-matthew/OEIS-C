/*
 * A299825  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers such that x <= y, x == y (mod 2), and |x+y-z|
 *          is a power of 4 (including 4^0 = 1).
 * https://oeis.org/A299825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = x² + y² + z² + w² with x,y,z,w
 *            ≥ 0, x ≤ y, x ≡ y (mod 2), and |x + y – z| a power of 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_power_of_four.h"

/* Integer square root: floor(sqrt(n)), n ≤ 2^64‑1. */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;   /* sqrt(2^64‑1) < 2^32 */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low  = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n ≥ 1 in practice */
    uint64_t N = n;
    seq_t count = 0;

    uint64_t max_xy = isqrt_uint64(N);
    for (uint64_t x = 0; x <= max_xy; ++x) {
        uint64_t x2 = x * x;
        if (x2 > N) break;
        for (uint64_t y = x; y <= max_xy; ++y) {
            /* same parity required: x ≡ y (mod 2) */
            if (((x ^ y) & 1U) != 0) continue;

            uint64_t y2 = y * y;
            uint64_t t = x2 + y2;
            if (t > N) break;      /* t increases with y */

            uint64_t rest = N - t;
            uint64_t max_z = isqrt_uint64(rest);
            for (uint64_t z = 0; z <= max_z; ++z) {
                uint64_t z2 = z * z;
                uint64_t rem = rest - z2;          /* z² + w² = rest */
                uint64_t w = isqrt_uint64(rem);
                if (w * w != rem) continue;        /* w not integer */

                uint64_t sum_xy = x + y;
                uint64_t diff = (sum_xy >= z) ? (sum_xy - z) : (z - sum_xy);
                if (is_power_of_four((int)diff))
                    count++;
            }
        }
    }
    return count;
}
