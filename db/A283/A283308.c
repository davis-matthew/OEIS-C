/*
 * A283308  List points (x,y) having integer coordinates, sorted first by
 *          x^2+y^2 and in case of ties, by x-coordinate and then by
 *          y-coordinate. Sequence gives y-coordinates.
 * https://oeis.org/A283308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = y-coordinate of the n-th integer point (x,y) ordered by
 *            distance squared (x²+y²), then by x, then by y
 * Range:     i8 n <= 87, i16 n <= 87, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (( (__int128)mid * mid ) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    if (n == 0) return 0; /* undefined for n<1, return 0 */

    for (idx_t r = 0; ; ++r) {
        idx_t lim = isqrt_u64(r);
        for (int64_t sx = -(int64_t)lim; sx <= (int64_t)lim; ++sx) {
            idx_t xsq = (idx_t)((int64_t)sx * (int64_t)sx);
            idx_t y2  = r - xsq;            /* non‑negative */
            idx_t y   = isqrt_u64(y2);
            if (( (__int128)y * y ) != y2)
                continue;                  /* not a perfect square */

            if (y == 0) {
                ++count;
                if (count == n)
                    return (seq_t)0;
            } else {
                seq_t yneg = -(seq_t)y;
                ++count;
                if (count == n)
                    return yneg;
                seq_t ypos = (seq_t)y;
                ++count;
                if (count == n)
                    return ypos;
            }
        }
    }
}
