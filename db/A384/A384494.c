/*
 * A384494  Triangle read by rows: T(n, k) = (-1)^k*(k+1)*(n+1-k)!, n >= 0, k
 *          = 0..n.
 * https://oeis.org/A384494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is obtained by reading triangle
 *            T(n,k)=(-1)^k*(k+1)*(n+1-k)! row by row. a(n) = T(r,k) where r
 *            is the unique non‑negative integer with (r+1)(r+2)/2 > n ≥
 *            (r)(r+1)/2 and k = n - (r)(r+1)/2
 * Range:     i8 n <= 14, i16 n <= 27, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   cum128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cum128.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th element. */
    idx_t r = 0;
    while (cum128(r) <= n)
        ++r;                     /* stop when cumulative > n */
    /* cumulative up to previous row */
    idx_t prev = (r == 0) ? 0 : (idx_t)cum128(r - 1);
    /* position k within the row */
    idx_t k = n - prev;
    /* factorial argument (r+1 - k) */
    idx_t m = (r + 1) - k;

    /* compute factorial m! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* sign = (-1)^k */
    seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;

    /* combine to get the term */
    return sign * fact * (seq_t)(k + 1);
}
