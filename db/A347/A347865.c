/*
 * A347865  Number of ways to write n as w^2 + 2*x^2 + y^4 + 3*z^4, where
 *          w,x,y,z are nonnegative integers.
 * https://oeis.org/A347865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions * of w^2 + 2*x^2
 *            + y^4 + 3*z^4 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t isqrt_u64(idx_t n) {
    /* Standard integer square root: returns floor(sqrt(n)) */
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t count = 0;

    for (idx_t y = 0;; ++y) {
        unsigned __int128 y4_128 = (unsigned __int128)y * y * y * y;
        if (y4_128 > n) break;
        idx_t y4 = (idx_t)y4_128;
        idx_t remY = n - y4;

        for (idx_t z = 0;; ++z) {
            unsigned __int128 z4_128 = (unsigned __int128)z * z * z * z;
            unsigned __int128 z4_3_128 = 3 * z4_128;
            if (z4_3_128 > remY) break;
            idx_t z4_3 = (idx_t)z4_3_128;
            idx_t rem = remY - z4_3;   /* remaining after y^4 and 3*z^4 */

            idx_t maxx = isqrt_u64(rem / 2);
            for (idx_t x = 0; x <= maxx; ++x) {
                unsigned __int128 term128 = (unsigned __int128)2 * x * x;
                idx_t term = (idx_t)term128;
                idx_t r = rem - term;   /* required w^2 = r */
                idx_t w = isqrt_u64(r);
                if (w * w == r) {
                    count++;
                }
            }
        }
    }
    return count;
}
