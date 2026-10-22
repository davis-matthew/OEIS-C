/*
 * A376215  Maximum number of non-overlapping discs with a diameter of 1 that
 *          can fit within an n X n square, using hexagonal packing or square
 *          packing, whichever is more efficient.
 * https://oeis.org/A376215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(n*n, n*floor(1+2*(n-1)/sqrt(3)) -
 *            floor(floor(1+2*(n-1)/sqrt(3))/2))
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   floor_div_by_sqrt3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_div_by_sqrt3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety; offset starts at 1 */
    idx_t k = 2 * (n - 1);
    idx_t m = floor_div_by_sqrt3(k);      /* floor(2*(n-1)/sqrt(3)) */
    idx_t t = m + 1;                      /* floor(1+2*(n-1)/sqrt(3)) */

    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t n_t    = (seq_t)n * (seq_t)t;
    seq_t half_t = (seq_t)(t >> 1);       /* floor(t/2) */
    seq_t n_hex  = n_t - half_t;          /* hexagonal packing count */

    return (n_sq > n_hex) ? n_sq : n_hex;
}
