/*
 * A259581  Reciprocity array of 3; rectangular, read by antidiagonals.
 * https://oeis.org/A259581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Sum_{k=0}^{m-1} ⌊(n*k+3)/m⌋, where (m,n) are the
 *            coordinates of the n‑th term in the antidiagonal (row, column)
 *            enumeration of the reciprocal array with x = 3
 * Range:     u8 n <= 1057, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   reciprocal_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reciprocal_T.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, undefined for n==0 */

    /* Find the antidiagonal sum 's' such that the total number of
       elements up to that sum is at least n.  The total elements up
       to sum s is triangular(s-1) = (s-1)*s/2.  We accumulate
       incrementally to avoid overflow or floating point. */
    idx_t s = 2;        /* minimal sum (1+1) */
    idx_t tri = 1;      /* number of elements up to sum s */

    while (tri < n) {
        ++s;            /* move to next antidiagonal */
        tri += s - 1;   /* add the length of the new row (s-1) */
    }

    /* Elements before the current diagonal: tri - (s-1) */
    idx_t prev = tri - (s - 1);
    idx_t pos = n - prev;                 /* position on current diagonal (1‑based) */
    idx_t m = pos;                        /* row index (1‑based) */
    idx_t col = s - pos;                  /* column index (1‑based) */

    return reciprocal_T(m, col);
}
