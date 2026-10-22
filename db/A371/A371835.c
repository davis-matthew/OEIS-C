/*
 * A371835  Triangle read by rows: T(n,k) is the number of points (x,y,z)
 *          satisfying |x|+|y|+|z|<=n and max(|x|,|y|,|z|)<=k; 0<=k<=n.
 * https://oeis.org/A371835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle T(n,k) read * by rows,
 *            where T(n,k) counts lattice points (x,y,z) with * |x|+|y|+|z| ≤
 *            n and max(|x|,|y|,|z|) ≤ k, 0 ≤ k ≤ n. * a(0) is the first term
 *            of the triangle
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   G2, G3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/G2.h"
#include "../utils/G3.h"

/* Find row r and column k such that
 * row r has r+1 entries and cumulative count up to row r-1 is
 * r*(r+1)/2 <= n < (r+1)*(r+2)/2.
 */
static inline void row_col_from_index(idx_t n, idx_t *row, idx_t *col)
{
    /* Simple linear search is fine for n ≤ 10⁴,
     * but we keep it generic using binary search. */
    idx_t lo = 0, hi = n * 2 + 2;  /* upper bound is safe */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* compute mid*(mid+1)/2 safely */
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is first index such that tri > n, so row = lo-1 */
    *row = lo - 1;
    *col = n - (*row * (*row + 1)) / 2;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    row_col_from_index(n, &r, &k);
    return G3(k, r);
}
