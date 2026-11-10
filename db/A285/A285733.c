/*
 * A285733  Transpose of square array A285732.
 * https://oeis.org/A285733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   A(n,k) = A285732(k,n); read by antidiagonals. With T(u,v) =
 *            (u+v-2)*(u+v-1)/2 + u (A000027 as a table): A(n,n) = -n; A(n,k)
 *            = T(n-k,k) if n > k; A(n,k) = T(n,k-n) if n < k
 * Range:     i8 n <= 136, i16 n <= 7260, i32 n <= 7260, i64 n <= 7260,
 *            i128 n <= 7260, big any n
 * Validated: standard, pass, 107 terms through n = 7252 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A285733_T(idx_t u, idx_t v)
{
    seq_t s = (seq_t)u + (seq_t)v;
    return (s - 2) * (s - 1) / 2 + (seq_t)u;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* smallest d with d(d+1)/2 >= n */
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n) hi = mid;
        else lo = mid;
    }
    idx_t d = hi;
    idx_t prev = (idx_t)((unsigned __int128)(d - 1) * d / 2);
    idx_t off = n - prev - 1;
    idx_t col0 = off + 1;      /* A285732 row */
    idx_t row0 = d - off;      /* A285732 col */
    /* transpose: r = row0, c = col0 */
    idx_t r = row0, c = col0;
    if (r == c) return -(seq_t)r;
    if (r > c) return A285733_T(r - c, c);
    return A285733_T(r, c - r);
}
