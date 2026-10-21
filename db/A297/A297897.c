/*
 * A297897  Triangular array read by row: T(m,n) = number of ways to obtain a
 *          single sphere by gluing the (labeled) sides of a (2m+1)-gon and a
 *          (2n+1)-gon, m >= n >= 0.
 * https://oeis.org/A297897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 34, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula for the triangular array:
        T(m,n) = (2*m*n + m + n + 1)/(m + n + 1) *
                 binomial(2*m+1,m) * binomial(2*n+1,n).
   Sequence a(n) is obtained by reading T row‑wise from top to bottom
   (first column 0 to m in each row).  The function below computes
   a(n) for any non‑negative n.
*/
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row index m such that m(m+1)/2 <= n < (m+1)(m+2)/2. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) {
        ++m;
    }

    idx_t prev = m * (m + 1) / 2;   /* number of terms before row m */
    idx_t p = n - prev;             /* column index within row m */

    /* Binomial coefficients for the two polygons. */
    seq_t bm = binomial(2 * m + 1, m);
    seq_t bn = binomial(2 * p + 1, p);

    /* Compute the numerator of the rational factor. */
    seq_t num = (seq_t)2 * m * p + m + p + 1;
    seq_t den = m + p + 1;

    /* Full result: (num * bm * bn) / den  (exact integer division). */
    seq_t prod = num;
    prod = prod * bm;
    prod = prod * bn;
    return prod / den;
}
