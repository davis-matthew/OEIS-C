/*
 * A248333  Number of unit squares enclosed by n lattice points in and along
 *          the first quadrant of the coordinate plane starting from (0,0) and
 *          moving along each square gnomon starting on the y-axis and ending
 *          on the x-axis.
 * https://oeis.org/A248333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - ⌊√(n-1)⌋ - ⌊(1+√(4n-3))/2⌋ (a(0)=0)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer.  Returns the greatest
 * integer r such that r*r <= x.  Uses binary search with 128‑bit
 * intermediate multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* floor sqrt(n-1)  */
    idx_t s1 = isqrt_u64(n - 1);

    /* floor((1+sqrt(4n-3))/2)  */
    idx_t val   = 4 * n - 3;               /* 4n-3  (n>=1) */
    idx_t s2   = (1 + isqrt_u64(val)) >> 1;

    idx_t special = s1 + s2;               /* number of indices that do not add a square */
    return (seq_t)n - (seq_t)special;     /* total squares up to n */
}
