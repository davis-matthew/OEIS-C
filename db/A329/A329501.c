/*
 * A329501  Array read by upward antidiagonals: row n = coordination sequence
 *          for cylinder formed by rolling up a strip of width n squares cut
 *          from the square grid by cuts parallel to grid lines.
 * https://oeis.org/A329501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   Array read by upward antidiagonals. * a(n) is obtained by
 *            traversing the infinite matrix whose * row r contains the
 *            coordination sequence for the cylinder of * width r (computed as
 *            described below) in order of upward * antidiagonals. This
 *            implementation uses the closed form * for the coordination...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static seq_t row_value(idx_t width, idx_t dist)
{
    if (dist == 0) return 1;                /* central vertex */
    /* maximum vertical distance */
    idx_t maxv = width / 2;                 /* floor(n/2) */
    idx_t lim = dist < maxv ? dist : maxv;  /* upper bound for v */
    seq_t sum = 0;

    for (idx_t v = 0; v <= lim; ++v)
    {
        seq_t cnt;
        if (v == 0) {
            cnt = 1;
        } else if ((width & 1) == 0 && v == width / 2) {
            cnt = 1;                         /* special case for even width */
        } else {
            cnt = 2;
        }

        seq_t term = cnt * ((dist == v) ? 1 : 2);
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* find the diagonal index t such that
     *  t*(t-1)/2 >= n  and (t-1)*(t-2)/2 < n
     */
    long double root = sqrt((long double)(8 * n + 1));
    idx_t t = (idx_t)((1 + root) / 2);           /* floor value */
    /* correct t to be the minimal satisfying the inequality */
    while ((t * (t - 1)) / 2 < n) ++t;

    idx_t prevS = ((t - 1) * (t - 2)) / 2;       /* elements before diag t */
    idx_t k = n - prevS - 1;                    /* 0‑based offset inside diag t */
    idx_t r = (t - 1) - k;                      /* row number */
    idx_t d = k;                                /* distance within that row */

    return row_value(r, d);
}
