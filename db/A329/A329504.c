/*
 * A329504  Array read by upward antidiagonals: row n = coordination sequence
 *          for cylinder formed by rolling up a strip of width n squares cut
 *          from the square grid by cuts at 45 degrees to grid lines.
 * https://oeis.org/A329504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth element of the array read by upward
 *            antidiagonals of rows r, where the row elements are given by:
 *            c_{r,0} = 1, c_{r,k} = 4k for 1 <= k <= r-1, c_{r,r} = 3r - 1,
 *            c_{r,k} = 2r for k > r. The sequence is obtained by traversing
 *            these rows along antidiagonals: for each diagonal d, elem...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* offset starts at 1 */

    /* Find the diagonal d such that T_{d-1} < n <= T_d   (T_d = d*(d+1)/2) */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;

    /* Number of elements before this diagonal */
    idx_t prev_tri = (d - 1) * d / 2;
    /* Position within current diagonal (1‑based) */
    idx_t pos = n - prev_tri;

    /* Coordinates: row r, column c */
    idx_t r = d - pos + 1;      /* 1‑based row index   */
    idx_t c = pos;              /* 1‑based column index */

    /* Convert to zero‑based column index for formula */
    idx_t k = c - 1;            /* k = 0,1,2,... */

    seq_t val;
    if (k == 0) {                  /* first element of each row */
        val = 1;
    } else if (k <= r - 1) {       /* 1 <= k <= r-1  */
        val = 4 * (seq_t)k;
    } else if (k == r) {           /* k == r          */
        val = 3 * (seq_t)r - 1;
    } else {                       /* k > r           */
        val = 2 * (seq_t)r;
    }

    return val;
}
