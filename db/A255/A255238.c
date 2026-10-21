/*
 * A255238  Triangle T(n, m) of numbers of points of a square lattice covered
 *          by a circular disk of radius n (centered at any lattice point
 *          taken as origin) with ordinate y = m in the first quadrant.
 * https://oeis.org/A255238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_row_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A255238: Flattened triangle of lattice points inside a circle.
 * a(n) = 1 + floor( sqrt( r^2 - c^2 ) )
 *   where r = floor_row_index(n)
 *         c = n - r*(r+1)/2
 *   and floor_row_index(n) is the unique integer r with r(r+1)/2 ≤ n < (r+1)(r+2)/2.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_row_index.h"

/* Integer square root (floor) for values that fit in 64 bits. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1) << 32;   /* high >= sqrt(ULLONG_MAX) */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Row index */
    idx_t r = floor_row_index(n);

    /* Number of terms before this row */
    __int128 before128 = (__int128)r * (r + 1) / 2;
    idx_t before = (idx_t)before128;

    /* Column index within the row */
    idx_t c = n - before;

    /* Compute floor(sqrt(r^2 - c^2)) */
    __int128 diff128 = (__int128)r * r - (__int128)c * c;
    idx_t diff = (idx_t)diff128;          /* diff ≥ 0 */

    idx_t sqrt_val = isqrt_u64(diff);

    return (seq_t)1 + (seq_t)sqrt_val;
}
