/*
 * A275409  Number of ordered ways to write n as 2*w^2 + x^2 + y^2 + z^2 with
 *          w + x + 2*y + 4*z a square, where w,x,y,z are nonnegative
 *          integers.
 * https://oeis.org/A275409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the number of ordered 4‑tuples (w,x,y,z) of
 * non‑negative integers satisfying
 *   n = 2*w^2 + x^2 + y^2 + z^2
 *   w + x + 2*y + 4*z  is a perfect square
 */
/* The implementation simply enumerates all possible w,y,z and
 * determines whether the remaining value is a perfect square.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a value of type idx_t (assumed to be 64‑bit).
 * Returns floor(sqrt(n)). */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t w = 0; ; ++w) {
        /* compute 2*w^2 using 128‑bit to avoid overflow */
        unsigned __int128 w2 = (unsigned __int128)w * w * 2;
        if (w2 > n) break;
        idx_t rem1 = (idx_t)((unsigned __int128)n - w2);

        for (idx_t y = 0; ; ++y) {
            unsigned __int128 y2 = (unsigned __int128)y * y;
            if (y2 > rem1) break;
            idx_t rem2 = (idx_t)((unsigned __int128)rem1 - y2);

            for (idx_t z = 0; ; ++z) {
                unsigned __int128 z2 = (unsigned __int128)z * z;
                if (z2 > rem2) break;
                idx_t rem3 = (idx_t)((unsigned __int128)rem2 - z2);

                /* rem3 must be x^2 */
                idx_t x = isqrt_idx(rem3);
                if ((unsigned __int128)x * x != rem3) continue;

                /* check the second condition */
                idx_t t = w + x + 2 * y + 4 * z;
                idx_t s = isqrt_idx(t);
                if ((unsigned __int128)s * s != t) continue;

                ++count;
            }
        }
    }
    return count;
}
