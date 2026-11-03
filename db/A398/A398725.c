/*
 * A398725  Number of increasing 3-term arithmetic progressions in an n^6
 *          hypercube.
 * https://oeis.org/A398725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((x^2 + y^2)^6 - n^6)/2, where x = floor(n/2) and y =
 *            ceil(n/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* split n into floor and ceil halves */
    idx_t x = n / 2;
    idx_t y = n - x;

    /* compute x^2 + y^2 */
    seq_t xsq = (seq_t)x * (seq_t)x;
    seq_t ysq = (seq_t)y * (seq_t)y;
    seq_t base = xsq + ysq;

    /* (x^2 + y^2)^6 and n^6 */
    seq_t pow6   = ipow_idx(base, 6);
    seq_t n_pow6 = ipow_idx((seq_t)n, 6);

    /* final formula */
    return (pow6 - n_pow6) / 2;
}
