/*
 * A303301  Square array T(n,k) read by antidiagonals upwards in which row n
 *          is obtained by taking the general formula for generalized n-gonal
 *          numbers: m*((n - 2)*m - n + 4)/2, where m = 0, +1, -1, +2, -2, +3,
 *          -3, ... and n >= 5. Here n >= 0.
 * https://oeis.org/A303301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,m) = m*((n-2)*m
 *            - n + 4)/2, where m = 0, +1, -1, +2, -2, +3, -3, ... (col‑index
 *            0,1,2,3,4,...) The array is traversed by antidiagonals upwards:
 *            (0,0),(1,0),(0,1),(2,0),(1,1),(0,2),…
 * Range:     i8 n <= 239, i16 n <= 7698, i32 n <= 9604, i64 n <= 9604,
 *            i128 n <= 9604, big any n
 * Helpers:   tri_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All arithmetic uses the seq_t type; idx_t is uint64_t. */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/tri_le.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    long double ld = sqrtl((long double)n * 8.0L + 1.0L);
    idx_t d = (idx_t)((ld - 1.0L) / 2.0L);

    while (tri_le(d + 1, n)) ++d;
    while (!tri_le(d, n)) --d;

    /* Offset of n within diagonal d. */
    idx_t prev_sum = (idx_t)(((__int128)d * (d + 1)) / 2);
    idx_t offset   = n - prev_sum;

    /* Coordinates in the array. */
    idx_t row_idx = d - offset;   /* row index n_row */
    idx_t col_idx = offset;       /* column index (pattern 0,+1,-1,+2,-2,…) */

    /* Map column index to m. */
    seq_t m;
    if (col_idx == 0) {
        m = 0;
    } else if (col_idx & 1U) {            /* odd: positive */
        m = (seq_t)((col_idx + 1) / 2);
    } else {                              /* even: negative */
        m = -(seq_t)(col_idx / 2);
    }

    /* Compute T(row_idx, m).  All arithmetic is in seq_t. */
    seq_t row = (seq_t)row_idx;
    seq_t part = (row - (seq_t)2) * m - row + (seq_t)4;
    seq_t res  = part * m;
    res = res / 2;
    return res;
}
