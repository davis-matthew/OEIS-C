/*
 * A393009  a(n) is the sum of mutual Manhattan distances of the A000328(n)
 *          distinct grid points (x_i,y_i) in a square lattice such that x_i^2
 *          + y_i^2 <= n^2.
 * https://oeis.org/A393009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – the sum of mutual Manhattan distances over the first A000328(n‑1)
 * lattice points with x² + y² <= (n‑1)².  The points are grouped by the
 * x‑coordinate.  For a given x the number of points is                    
 * m = 2*floor(sqrt((n‑1)² – x²)) + 1.  The sum of |x_i – x_j| over all
 * unordered pairs is accumulated incrementally; the y‑coordinate contributes
 * the same amount, so the final result is 2 × that accumulated sum.        */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    idx_t R = n - 1;                                 /* radius */
    signed __int128 C = 0;                           /* total points processed so far */
    signed __int128 sumPrevX = 0;                    /* Σ (x * multiplicity) for processed points */
    signed __int128 totalDiff = 0;                   /* Σ_{i<j} |x_i - x_j|  (over all pairs) */

    /* iterate x from -R to R inclusive */
    for (uint64_t xi_idx = 0; xi_idx <= 2 * R; ++xi_idx) {
        int64_t x = (int64_t)xi_idx - (int64_t)R;            /* current x coordinate */

        int64_t sq = (int64_t)R * (int64_t)R - (int64_t)x * (int64_t)x;
        idx_t sqrtVal = isqrt_u64((idx_t)sq);
        idx_t m = 2 * sqrtVal + 1;                           /* multiplicity of this x */

        signed __int128 curr = (signed __int128)x;
        signed __int128 diff = C * curr - sumPrevX;          /* contribution of this x to the sum */
        totalDiff += diff * m;                              /* add weighted diff */

        C += m;                                             /* update counts */
        sumPrevX += curr * m;
    }

    /* The y‑coordinate gives the same contribution, so multiply by 2. */
    signed __int128 result128 = totalDiff * 2;
    return (seq_t)result128;                                 /* cast to seq_t (wrap/overflow as needed) */
}
