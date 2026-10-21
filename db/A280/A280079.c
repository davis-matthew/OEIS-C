/*
 * A280079  Abscissa of points (x,y) of the square lattice such that x >= 0
 *          and 0 <= y <= x, and ranked in order of increasing distance from
 *          the origin. Equidistant points are ranked in order of increasing
 *          ordinate.
 * https://oeis.org/A280079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280079: a(n) is the x‑coordinate of the n‑th point (x,y)
   with x ≥ 0 and 0 ≤ y ≤ x, ordered by increasing Euclidean distance
   from the origin; points with the same distance are ordered by increasing
   ordinate (y). The function computes the term via a binary search on
   distance squared and a simple enumeration of the points at that
   distance. */
 
#include <stdint.h>
#include <stdlib.h>
 
/* Integer floor‑square‑root of an unsigned 64‑bit integer. */
/* Uses binary search on __int128 to avoid overflow when squaring. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}
 
/* Count how many points (x,y) with 0 ≤ y ≤ x satisfy x²+y² ≤ D. */
static idx_t count_points(idx_t D)
{
    idx_t maxx = isqrt_u64(D);
    idx_t cnt = 0;
    for (idx_t x = 0; x <= maxx; ++x) {
        unsigned __int128 xsq = (unsigned __int128)x * x;
        idx_t rem = D - (idx_t)xsq;   /* D ≥ xsq, so safe */
        idx_t ymax = isqrt_u64(rem);
        if (ymax > x) ymax = x;
        cnt += ymax + 1;
    }
    return cnt;
}
 
seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* the sequence is 1‑indexed; below offset is undefined */
 
    /*--- Find minimal D such that count_points(D) ≥ n via binary search ---*/
    idx_t low = 0;
    idx_t high = (idx_t)(n * 3ULL) + 1;   /* initial high; 3×n is a safe start */
    while (count_points(high) < n) high <<= 1;      /* grow if necessary */
 
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (count_points(mid) >= n) high = mid;
        else                        low = mid + 1;
    }
    idx_t D = low;                    /* minimal distance‑squared covering n points */
 
    /*--- Determine which point of distance D is the n‑th ---*/
    idx_t prev_cnt = 0;
    if (D > 0) prev_cnt = count_points(D - 1);
    idx_t offset = n - prev_cnt - 1;   /* 0‑based offset among points at distance D */
 
    /*--- Enumerate all points with distance squared D, sorted by y (ascending) ---*/
    for (idx_t y = 0; ; ++y) {
        unsigned __int128 rem128 = (unsigned __int128)D - (unsigned __int128)y * y;
        if (rem128 > UINT64_MAX) continue;        /* should not happen for current bounds */
        idx_t rem = (idx_t)rem128;
        idx_t x = isqrt_u64(rem);
        if ((unsigned __int128)x * x != rem128) continue;   /* not a perfect square */
        if (x < y) continue;                                 /* ensure 0 ≤ y ≤ x */
        if (offset == 0) return (seq_t)x;
        offset--;
    }
 
    /* Should never reach here */
    return 0;
}
