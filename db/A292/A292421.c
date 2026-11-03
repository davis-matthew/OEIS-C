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
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline seq_t tile_cross(idx_t h, idx_t k)
{
    seq_t sum = 0;
    idx_t y;
    for (y = 0; y < k; ++y) {
        if ((y & 1ULL) == 0) {                 /* even row (offset 0) */
            unsigned __int128 prod1 = (unsigned __int128)h * (y + 1);
            unsigned __int128 prod2 = (unsigned __int128)h * y;
            seq_t q1 = (seq_t)((prod1 - 1) / k);
            seq_t q2 = (seq_t)(prod2 / k);
            sum += (seq_t)(q1 - q2 + 1);
        } else {                                /* odd row (offset 1/2) */
            unsigned __int128 prod1 = (unsigned __int128)h * (y + 1);
            unsigned __int128 prod2 = (unsigned __int128)h * y;
            unsigned __int128 num1 = 2 * prod1 + k;         /* 2h(y+1)+k */
            unsigned __int128 num2 = 2 * prod2 + k;         /* 2hy+k    */
            unsigned __int128 den  = (unsigned __int128)2 * k;
            seq_t q1 = (seq_t)((num1 - 1) / den);
            seq_t q2 = (seq_t)(num2 / den);
            sum += (seq_t)(q1 - q2 + 1);
        }
    }
    return sum;
}

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
