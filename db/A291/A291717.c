/*
 * A291717  Triangle T(m,k) read by rows, where T(m,k) is the number of ways
 *          in which 1 <= k <= m positions can be picked in an m X m square
 *          grid such that the picked positions have a central symmetry.
 * https://oeis.org/A291717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   compute the number of k‑element subsets of an m×m grid that are
 *            invariant under 180° rotation about some centre. The sequence
 *            a(n) enumerates the triangle T(m,k) by rows, where n maps to
 *            (m,k) with 1 ≤ k ≤ m
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 89, u64 n <= 252,
 *            u128 n <= 778, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* auxiliary: power of two cast for abs calculation */
#define ABS_VAL(x) ((x) > 0 ? (x) : -(x))

seq_t a(idx_t n)
{
    /* Find the row m such that (m-1)*m/2 < n ≤ m*(m+1)/2 */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;
    idx_t prev = (m - 1) * m / 2;
    idx_t k = n - prev;

    /* Special case: k odd (must have a central cell) */
    if (k & 1) {
        idx_t t = (k - 1) >> 1;       /* number of pairs needed */
        seq_t sum = 0;
        /* iterate over grid cells (centre must be a grid point) */
        for (idx_t cx = 1; cx <= m; ++cx) {
            idx_t a = 2 * cx;
            idx_t absx = ABS_VAL((int)(a - (m + 1)));
            if (absx >= m) continue;       /* no pairs with this cx */
            idx_t Lx = m - absx;

            for (idx_t cy = 1; cy <= m; ++cy) {
                idx_t b = 2 * cy;
                idx_t absy = ABS_VAL((int)(b - (m + 1)));
                if (absy >= m) continue;    /* no pairs with this cy */
                idx_t Ly = m - absy;

                idx_t prod = Lx * Ly;        /* |S| of points with this centre */
                if (prod == 0) continue;
                idx_t M = (prod - 1) >> 1;   /* pairs having this centre (cell centre) */
                if (M < t) continue;
                sum += binom(M, t);
            }
        }
        return sum;
    }

    /* Even k: centre may be integer or half‑integer, no fixed point */
    idx_t t = k >> 1;                     /* number of pairs needed */
    seq_t sum = 0;
    for (idx_t a = 2; a <= 2 * m; ++a) {  /* a = 2*cx, runs over integers and odds */
        idx_t absx = ABS_VAL((int)(a - (m + 1)));
        if (absx >= m) continue;
        idx_t Lx = m - absx;

        for (idx_t b = 2; b <= 2 * m; ++b) {
            idx_t absy = ABS_VAL((int)(b - (m + 1)));
            if (absy >= m) continue;
            idx_t Ly = m - absy;

            idx_t prod = Lx * Ly;        /* |S| for this centre */
            if (prod == 0) continue;
            idx_t M = prod >> 1;         /* pairs sharing this centre */
            if (M < t) continue;
            sum += binom(M, t);
        }
    }
    return sum;
}
