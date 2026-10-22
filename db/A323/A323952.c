/*
 * A323952  Regular triangle read by rows where if k > 1 then T(n, k) is the
 *          number of connected subgraphs of an n-cycle with any number of
 *          vertices other than 2 through k - 1, n >= 1, 1 <= k <= n - 1.
 *          Otherwise T(n, 1) = n.
 * https://oeis.org/A323952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) * where r = smallest integer with r(r+1)/2 ≥ n, *
 *            k = n - r(r-1)/2, * T(r,1) = r, * T(r,k) = 1 + r*(r-k+1) for k>1
 * Range:     u8 n <= 137, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* integer square root of x (floor) using binary search */
static idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x)
            return mid;
        else if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi; /* floor sqrt */
}

seq_t a(idx_t n)
{
    if (n == 1) { /* quick path for first term */
        return (seq_t)1;
    }

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = (int_sqrt(1 + (__int128)8 * n) - 1) / 2;

    /* adjust in case the floor sqrt was not enough */
    __int128 tri = (__int128)r * (r + 1) / 2;
    if (tri < n) {
        ++r;
        tri = (__int128)r * (r + 1) / 2;
    }

    /* compute k = position within the row */
    idx_t k = n - (idx_t)(((__int128)(r - 1) * r) / 2);

    /* compute the triangle value */
    if (k == 1)
        return (seq_t)r;
    return (seq_t)(1 + (seq_t)r * (seq_t)(r - k + 1));
}
