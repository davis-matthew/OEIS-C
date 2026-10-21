/*
 * A292421  Square array T(n,k) = number of tiles crossed by the line segment
 *          (0,0) -- (n,k) in a running bond pattern tiling with square tiles,
 *          read by antidiagonals.
 * https://oeis.org/A292421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i+j-1=n; T(i,j) counts tiles crossed by the
 *            segment (0,0)-(i,j) in a running bond tiling
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   tile_cross
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/tile_cross.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal s such that (s‑1)*s/2 ≥ n */
    long double disc = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t s = (idx_t)((1.0L + disc) / 2.0L);  /* floor result */

    /* Correct possible rounding errors */
    while ((unsigned __int128)(s - 1) * s / 2 < n) ++s;

    idx_t t_prev = (s >= 2) ? ((s - 2) * (s - 1) / 2) : 0;
    idx_t i = n - t_prev;          /* column index   */
    idx_t j = s - i;               /* row index      */

    return tile_cross(i, j);
}
