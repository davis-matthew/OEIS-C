/*
 * A275344  Number of ordered ways to write n as x^2 + y^2 + z^2 + 2*w^2 with
 *          x + 2*y + 3*z a square, where x,y,z,w are nonnegative integers.
 * https://oeis.org/A275344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered quadruples (x,y,z,w) of non‑negative *
 *            integers satisfying * x^2 + y^2 + z^2 + 2w^2 = n * x + 2y + 3z
 *            is a perfect square
 * Range:     u8 n <= 5116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(v)) for 0 <= v <= 10^6 (safe for
 * all values used in this routine). */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t w = 0; ; ++w) {
        idx_t w2   = w * w;    /* w^2                      */
        idx_t val2 = 2 * w2;   /* 2*w^2                    */
        if (val2 > n) break;   /* stop when 2*w^2 > n       */
        idx_t t = n - val2;    /* remaining after w         */

        for (idx_t z = 0; ; ++z) {
            idx_t z2 = z * z;   /* z^2                      */
            if (z2 > t) break;  /* stop when z^2 > remaining*/
            idx_t t2 = t - z2;  /* remaining after z        */

            for (idx_t y = 0; y * y <= t2; ++y) {
                idx_t y2 = y * y;
                idx_t x2 = t2 - y2;          /* remaining after y    */
                idx_t x  = isqrt_u64(x2);
                if (x * x != x2) continue;   /* x must be integer    */

                idx_t s = x + 2 * y + 3 * z; /* sum that must be square  */
                idx_t r = isqrt_u64(s);
                if (r * r == s)              /* square check          */
                    count += 1;
            }
        }
    }
    return count;
}
