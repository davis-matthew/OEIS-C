/*
 * A266968  Number of ordered ways to write n as x^5+y^4+z^3+w*(w+1)/2, where
 *          x, y, z and w are nonnegative integers with z > 0 and w > 0.
 * https://oeis.org/A266968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered quadruples (x, y, z, w) such that * x⁵
 *            + y⁴ + z³ + w·(w+1)/2 = n, * with x, y, w ≥ 0 and z > 0, w > 0
 * Range:     u8 n <= 32, u16 n <= 9064, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute floor(sqrt(x)) for arbitrary unsigned integer type seq_t. */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;          /* for next iteration */
            continue;
        }
        seq_t div = x / mid;
        if (mid > div) {
            high = mid - 1;
        } else {
            if (mid * mid == x) {
                return mid;
            }
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t x = 0; ; ++x) {
        seq_t x5 = (seq_t)x * (seq_t)x * (seq_t)x * (seq_t)x * (seq_t)x; /* x⁵ */
        if (x5 > n) break;

        for (idx_t y = 0; ; ++y) {
            seq_t y4 = (seq_t)y * (seq_t)y * (seq_t)y * (seq_t)y;      /* y⁴ */
            seq_t sum_xy = x5 + y4;
            if (sum_xy > n) break;

            for (idx_t z = 1; ; ++z) {
                seq_t z3 = (seq_t)z * (seq_t)z * (seq_t)z;              /* z³ */
                seq_t sum_xyz = sum_xy + z3;
                if (sum_xyz > n) break;

                seq_t rem = n - sum_xyz;                               /* remaining for triangular number */
                if (rem == 0) continue;                               /* w must be > 0 */

                seq_t disc = rem * 8 + 1;                              /* 1 + 8·rem */
                seq_t s = isqrt_seq(disc);
                if (s * s == disc && (s & 1) == 1) {                 /* disc is a perfect square and s odd */
                    /* w = (s - 1)/2; w > 0 guaranteed because rem > 0 */
                    ++count;
                }
            }
        }
    }
    return count;
}
