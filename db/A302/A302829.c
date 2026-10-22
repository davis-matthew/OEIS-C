/*
 * A302829  a(n) is the number of lattice points in a Cartesian grid between a
 *          circle of radius n and an inscribed square whose vertices lie on
 *          the coordinate axes.
 * https://oeis.org/A302829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (x,y) ∈ ℤ² : x² + y² < n² and |x| + |y| > n }
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts lattice points inside the circle of radius n
   that lie outside the rotated square |x|+|y| ≤ n.  The code
   evaluates the counts by iterating over a single quadrant and
   exploiting symmetry.  An integer square root routine is
   provided locally to avoid any external dependencies.  */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 ≤ n ≤ 2⁶⁴−1.
   Uses binary search and fits within a 64‑bit word.  */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    idx_t res  = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq <= n) {
            res  = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* safety guard – offset is 1, but n=0 is harmless */
        return 0;

    const idx_t n2      = n * n;
    const idx_t n2m1    = n2 - 1;          /* strict inequality: x² + y² < n² */

    seq_t total_inside = 0;   /* points with x² + y² < n²            */
    seq_t total_both   = 0;   /* points with x² + y² < n²  and  |x|+|y| ≤ n */

    for (idx_t x = 0; x < n; ++x) {
        idx_t xsq   = x * x;
        idx_t t     = n2m1 - xsq;           /* non‑negative for 0 ≤ x < n */
        idx_t ymax  = isqrt_u64(t);         /* largest |y| with x² + y² < n² */

        /* Count of y‑values for this particular |x| (including y=0) */
        seq_t count_in_circle = ( (seq_t)2 * (seq_t)ymax ) + 1;

        /* For points also satisfying |x|+|y| ≤ n
           the bound on |y| is min(n - x, ymax).  */
        idx_t smin  = n - x;
        idx_t lim   = smin < ymax ? smin : ymax;
        seq_t count_both = ( (seq_t)2 * (seq_t)lim ) + 1;

        if (x == 0) {                 /* singular x = 0 axis */
            total_inside += count_in_circle;
            total_both   += count_both;
        } else {                      /* double for x = ±x */
            total_inside += count_in_circle * 2;
            total_both   += count_both   * 2;
        }
    }

    return total_inside - total_both;
}
