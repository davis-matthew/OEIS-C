/*
 * A273021  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with
 *          2*x*y + y*z - z*w - w*x a square, where w is a positive integer
 *          and x,y,z are nonnegative integers with x <= y.
 * https://oeis.org/A273021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ordered quadruples (x,y,z,w) with w>0,
 * x,y,z ≥ 0, x ≤ y, x² + y² + z² + w² = n,
 * and 2*x*y + y*z - z*w - w*x is a perfect square.
 * The function performs a brute‑force search that is
 * O(n³/16) in practice and fast enough for n ≤ 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Integer square root for 64‑bit unsigned values, correct up to 2^63-1. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t r = (idx_t)floor(sqrt((long double)n));
    /* Adjust for possible rounding errors. */
    while ((__int128)(r + 1) * (r + 1) <= n) ++r;
    while ((__int128)r * r > n) --r;
    return r;
}

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset starts at 1 */

    uint64_t cnt = 0;      /* use 64‑bit intermediate to avoid overflow */

    idx_t w_max = isqrt_u64(n);
    for (idx_t w = 1; w <= w_max; ++w)
    {
        idx_t w2 = (idx_t)((__int128)w * w);
        idx_t rem_w = n - w2;
        idx_t x_max = isqrt_u64(rem_w);
        for (idx_t x = 0; x <= x_max; ++x)
        {
            idx_t x2 = (idx_t)((__int128)x * x);
            idx_t rem_x = rem_w - x2;
            idx_t y_max = isqrt_u64(rem_x);
            for (idx_t y = x; y <= y_max; ++y)
            {
                idx_t y2 = (idx_t)((__int128)y * y);
                idx_t rem_y = rem_x - y2;
                idx_t z = isqrt_u64(rem_y);
                if ((idx_t)((__int128)z * z) != rem_y)
                    continue;           /* z not integral */

                /* Check if 2*x*y + y*z - z*w - w*x is a square. */
                __int128 P128 = (__int128)2 * (__int128)x * (__int128)y
                                + (__int128)y * (__int128)z
                                - (__int128)z * (__int128)w
                                - (__int128)w * (__int128)x;
                if (P128 < 0)
                    continue;
                idx_t P = (idx_t)P128;
                idx_t sqrtP = isqrt_u64(P);
                if ((__int128)sqrtP * sqrtP == P)
                    ++cnt;
            }
        }
    }
    return (seq_t)cnt;
}
