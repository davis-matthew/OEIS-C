/*
 * A350857  Number of ways to write n as 2*w^4 + x^4 + y^2 + z^2, where
 *          w,x,y,z are nonnegative integers with w or x a square.
 * https://oeis.org/A350857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer tuples (w,x,y,z)≥0 such that * n =
 *            2*w^4 + x^4 + y^2 + z^2 and w or x is a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)n) return mid;
        if (sq < (unsigned __int128)n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* determine limits for w and x */
    idx_t w_limit = isqrt_u64(isqrt_u64(n / 2));
    idx_t x_limit = isqrt_u64(isqrt_u64(n));

    for (idx_t w = 0; w <= w_limit; ++w)
    {
        idx_t w_sq  = w * w;                 /* w^2   */
        unsigned __int128 w4  = (unsigned __int128)w_sq * w_sq;  /* w^4  */
        unsigned __int128 term_w = 2 * w4;   /* 2*w^4 */
        if (term_w > (unsigned __int128)n) break;   /* further w only increase */

        bool w_square = w == isqrt_u64(w) * isqrt_u64(w);

        for (idx_t x = 0; x <= x_limit; ++x)
        {
            idx_t x_sq   = x * x;                 /* x^2   */
            unsigned __int128 x4   = (unsigned __int128)x_sq * x_sq;  /* x^4  */
            unsigned __int128 term = term_w + x4;      /* 2*w^4 + x^4 */

            if (term > (unsigned __int128)n) break;   /* further x only increase */

            bool x_square = x == isqrt_u64(x) * isqrt_u64(x);
            if (!w_square && !x_square) continue;    /* need at least one square */

            idx_t m = n - (idx_t)term;   /* remaining for y^2 + z^2 */

            idx_t y_limit = isqrt_u64(m);
            for (idx_t y = 0; y <= y_limit; ++y)
            {
                idx_t y_sq = y * y;
                idx_t rem  = m - y_sq;       /* possible z^2 */
                idx_t z     = isqrt_u64(rem);
                if ((idx_t)z * (idx_t)z == rem && y <= z)
                    ++count;
            }
        }
    }

    return count;
}
