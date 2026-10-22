/*
 * A393008  a(n) is the sum of mutual Manhattan distances of the A117609(n)
 *          distinct grid points (x_i,y_i,z_i) in a cubic lattice such that
 *          x_i^2 + y_i^2 + z_i^2 <= n.
 * https://oeis.org/A393008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * sum over unordered pairs of |x_i - x_j| for all
 *            lattice points (x_i,y_i,z_i) with x_i^2 + y_i^2 + z_i^2 <= n
 *            This uses symmetry: the sum over |y_i - y_j| and |z_i - z_j| are
 *            equal to that over x
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 179, u64 n <= 179,
 *            u128 n <= 179, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"   /* declares _floor_isqrt_u64(idx_t) */

seq_t a(idx_t n)
{
    /* Radius of the sphere */
    idx_t R = _floor_isqrt_u64(n);

    /* Frequency array for x-coordinates: indexed by x+R, size = 2*R+1 */
    /* R <= floor(sqrt(10000)) = 100, so size 201 is safe */
    static seq_t freq[201];
    for (idx_t i = 0; i <= 2*R; ++i) freq[i] = 0;

    /* Count points per x-coordinate */
    for (idx_t ix = 0; ix <= 2*R; ++ix) {
        long long x = (long long)ix - (long long)R;     /* signed x value  */
        long long xsq = x * x;
        if (xsq > (long long)n) continue;              /* no y possible */

        idx_t maxY = _floor_isqrt_u64(n - (idx_t)xsq);
        for (idx_t iy = 0; iy <= 2*maxY; ++iy) {
            long long y = (long long)iy - (long long)maxY;   /* signed y value */
            long long ysq = y * y;
            long long sumxy = xsq + ysq;
            if (sumxy > (long long)n) continue;

            idx_t rem   = n - (idx_t)sumxy;
            idx_t zmax  = _floor_isqrt_u64(rem);
            seq_t cnt   = (seq_t)(2*zmax + 1);     /* z runs from -zmax..zmax */
            freq[ix] += cnt;
        }
    }

    /* Compute the sum over |x_i - x_j| using prefix sums */
    long long prefCount = 0;   /* total points processed so far   */
    long long prefSumX  = 0;   /* sum of (index_value * count)   */
    seq_t     S          = 0;   /* sum over |x_i - x_j|          */

    for (idx_t ix = 0; ix <= 2*R; ++ix) {
        seq_t  m     = freq[ix];
        if (m == 0) continue;
        long long x   = (long long)ix - (long long)R;        /* signed x */
        long long mi  = (long long)m;

        /* Contribution from all pairs with previous x's */
        long long contrib = (x * prefCount - prefSumX) * mi;
        S += (seq_t)contrib;

        /* Update running totals */
        prefCount += mi;
        prefSumX  += x * mi;
    }

    /* Manhattan distance sum = 3 * S */
    return S * (seq_t)3;
}
