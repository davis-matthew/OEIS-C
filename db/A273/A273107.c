/*
 * A273107  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with
 *          (8*x+12*y)^2 + (15*z)^2 a square, where x,y,z,w are nonnegative
 *          integers with x+y > 0 and z > 0.
 * https://oeis.org/A273107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered quadruples (x,y,z,w) of non‑negative
 *            integers with x + y > 0, z > 0 satisfying n = x² + y² + z² + w²
 *            and (8x + 12y)² + (15z)² is a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer, floor(sqrt(n)) */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t op  = n;
    uint64_t bit = 1ULL << 62;          /* highest power of four <= 2⁶⁴ */

    while (bit > op) bit >>= 2;         /* shift until bit <= op */

    while (bit != 0) {
        if (op >= res + bit) {
            op -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety: sequence defined for n≥1 */

    uint64_t N     = (uint64_t)n;      /* all arithmetic done in 64‑bit */
    uint64_t count = 0;

    uint64_t max_z = isqrt_u64(N);
    for (uint64_t z = 1; z <= max_z; ++z) {
        uint64_t z2 = z * z;
        if (z2 > N) break;
        uint64_t rem_z = N - z2;
        uint64_t b      = 15 * z;
        uint64_t b2     = b * b;

        uint64_t max_y = isqrt_u64(rem_z);
        for (uint64_t y = 0; y <= max_y; ++y) {
            uint64_t y2 = y * y;
            if (y2 > rem_z) break;
            uint64_t rem_y = rem_z - y2;

            uint64_t a12  = 12 * y;            /* part of (8x+12y) */
            uint64_t max_x = isqrt_u64(rem_y);
            for (uint64_t x = 0; x <= max_x; ++x) {
                if (x == 0 && y == 0) continue; /* require x + y > 0 */
                uint64_t x2 = x * x;
                if (x2 > rem_y) break;
                uint64_t w2 = rem_y - x2;
                uint64_t w   = isqrt_u64(w2);
                if (w * w != w2) continue;      /* w must be integer */

                uint64_t a_val = 8 * x + a12;
                uint64_t a2     = a_val * a_val;
                uint64_t sum_ab2 = a2 + b2;
                uint64_t t        = isqrt_u64(sum_ab2);
                if (t * t == sum_ab2) {          /* perfect‑square condition */
                    ++count;
                }
            }
        }
    }
    return (seq_t)count;
}
