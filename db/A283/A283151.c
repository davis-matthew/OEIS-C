/*
 * A283151  Triangle read by rows: Riordan array (1/(1-9x)^(2/3), x/(9x-1)).
 * https://oeis.org/A283151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is obtained by reading the triangle * defined by the
 *            Riordan array (1/(1-9x)^(2/3), x/(9x-1)) row by row. * In the
 *            triangle, a(m,k) satisfies * a(m,0) = a(m-1,0) * 3*(3*m-1)/m
 *            (with a(0,0)=1) * a(m,k) = a(m,k-1) * (k-1-m) / (9*k-3) for
 *            1≤k≤m * The sequence a(n) is a(m,k) where the index n ma...
 * Range:     i8 n <= 5, i16 n <= 20, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   compute_row_first
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/compute_row_first.h"

seq_t a(idx_t n)
{
    /* Determine row m and column k such that the n‑th term is a(m,k). */
    idx_t m = (idx_t)((sqrt(8.0L * n + 1.0L) - 1.0L) / 2.0L);
    /* Correct possible off‑by‑one caused by the floating point calculation. */
    while ((m+1)*(m+2)/2 <= n) ++m;
    while (m*(m+1)/2 > n) --m;

    idx_t prev = m*(m+1)/2;   /* cumulative count up to row m-1 */
    idx_t k = n - prev;       /* column within the row */

    seq_t val = compute_row_first(m);
    for (idx_t col = 1; col <= k; ++col) {
        int64_t num = (int64_t)col - 1 - (int64_t)m;   /* (col-1-m) */
        seq_t denom = (seq_t)(9*col - 3);              /* (9*col-3) */
        val = val * (seq_t)num / denom;
    }
    return val;
}
