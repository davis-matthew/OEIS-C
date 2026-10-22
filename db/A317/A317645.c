/*
 * A317645  Expansion of (1 + theta_3(q))^3*(1 + theta_3(q^2))/16, where
 *          theta_3() is the Jacobi theta function.
 * https://oeis.org/A317645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of nonnegative integer solutions to * x^2 + y^2 + z^2 +
 *            2*w^2 = n * (OEIS A317645)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) */
static inline idx_t isqrt(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = 1; continue; }
        if (mid <= n / mid) {           /* mid*mid <= n  */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    /* iterate over w */
    for (idx_t w = 0;; ++w) {
        idx_t w2 = w * w;
        /* terminate when 2*w^2 > n */
        if (2 * w2 > n) break;
        idx_t m = n - 2 * w2;           /* remaining to be represented by x^2 + y^2 + z^2 */

        /* iterate over x */
        idx_t max_x = isqrt(m);
        for (idx_t x = 0; x <= max_x; ++x) {
            idx_t x2 = x * x;
            idx_t remx = m - x2;        /* y^2 + z^2 */

            /* iterate over y */
            idx_t max_y = isqrt(remx);
            for (idx_t y = 0; y <= max_y; ++y) {
                idx_t y2 = y * y;
                idx_t zsq = remx - y2;  /* z^2 */

                idx_t z = isqrt(zsq);
                if (z * z == zsq) {
                    count++;               /* (x, y, z) found */
                }
            }
        }
    }
    return count;
}
