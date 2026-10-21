/*
 * A279416  Triangle read by rows: T(n,k), n >= k >= 1, is the number of grid
 *          points below the diagonal of an n X k grid.
 * https://oeis.org/A279416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r = ceil((sqrt(1+8*n)-1)/2), c = n -
 *            r*(r-1)/2; T(r,c) = (c*r - (gcd(r-1,c-1)+1))/2
 * Range:     u8 n <= 296, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * mid <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r-1)/2 < n <= r*(r+1)/2 */
    idx_t r = (isqrt_u64(1 + 8 * n) - 1) / 2;
    if (r * (r + 1) / 2 < n) r++;

    idx_t prev_cum = r * (r - 1) / 2;
    idx_t c = n - prev_cum;         /* 1 <= c <= r */

    seq_t rr = (seq_t)r;
    seq_t cc = (seq_t)c;
    seq_t prod = rr * cc;
    seq_t g = gcd((seq_t)(r - 1), (seq_t)(c - 1));
    seq_t val = prod - (g + 1);
    val /= 2;
    return val;
}
