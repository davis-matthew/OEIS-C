/*
 * A267864  Denominator triangle for A267863: T(n, k) = denominator((n -
 *          2*k)/(2*n)), n >= 1, k = 1, ..., n.
 * https://oeis.org/A267864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabl, easy
 * Formula:   a(n) = denominator((r - 2*c)/(2*r)) where r, c are the row and
 *            column of the triangle T(n,k), obtained by flattening the
 *            denominator triangle T(n,k) = denominator((n-2k)/(2n))
 *            row‑by‑row
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   gcd, prev_row_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/prev_row_sum.h"

/* Find the row r such that the n-th term lies in row r
   of the triangle.  Row r contains exactly r terms. */
static inline idx_t triangle_row(idx_t n)
{
    idx_t low = 1, high = n;                 /* r ∈ [1, n] */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;                               /* r */
}

seq_t a(idx_t n)
{
    /* Get row r of the triangle */
    idx_t r = triangle_row(n);

    /* Column index c (1‑based) in that row */
    idx_t prev = prev_row_sum(r);
    idx_t c = n - prev;                 /* 1 <= c <= r */

    /* Compute the fraction (r - 2c)/(2r) */
    signed __int128 numer = (signed __int128)r - 2 * (signed __int128)c;
    seq_t denom = (seq_t)((unsigned __int128)2 * r);   /* 2r */

    /* If numerator is zero, denominator is 1. */
    if (numer == 0)
        return 1;

    /* Absolute value of numerator as seq_t */
    seq_t numer_abs = numer > 0 ? (seq_t)numer : (seq_t)(-numer);

    /* Reduce fraction: denominator / gcd(numer_abs, denom) */
    seq_t g = gcd( numer_abs, denom );
    return denom / g;
}
