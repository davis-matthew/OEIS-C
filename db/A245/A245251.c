/*
 * A245251  Triangle T(a,b) read by rows: Sum_{k=1..a-1} floor(k*b/a).
 * https://oeis.org/A245251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = (a*b - a - b + gcd(a,b))/2, where (a,b) are the row and
 *            column indices (starting at 1) satisfying (a-1)*a/2 < n <=
 *            a*(a+1)/2 and b = n - (a-1)*a/2
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t a_row = 1;   /* row index */
    idx_t cum   = 0;   /* total terms in previous rows */

    /* Find the row containing the nth term */
    while (cum + a_row < n) {
        cum += a_row;
        ++a_row;
    }

    idx_t b = n - cum;          /* column index within the row */

    seq_t sa = (seq_t)a_row;
    seq_t sb = (seq_t)b;
    seq_t g  = gcd(sa, sb);

    return (sa * sb - sa - sb + g) / 2;
}
