/*
 * A360967  Array T(n,m) = (2^(m*(2*n+1))+1)/(2^m+1) read by antidiagonals.
 * https://oeis.org/A360967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = (2^(m*(2*n+1)) + 1) / (2^m + 1), read by antidiagonals
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 23, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the nth term of OEIS A360967. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1u) return 0;   /* offset is 1, undefined for n < 1 */

    /* Find the diagonal d such that d*(d-1)/2 >= n
       (diagonals are indexed by d = n + m, starting at 2) */
    idx_t d = 2;
    while ((d * (d - 1)) / 2 < n) {
        ++d;
    }

    /* Number of terms before this diagonal */
    idx_t prev = (d - 1) * (d - 2) / 2;

    /* Zero‑based position inside the diagonal */
    idx_t pos = n - prev - 1;

    /* Row index (n) and column index (m) of the array element */
    idx_t row = pos + 1;    /* array row is 1‑based */
    idx_t col = d - row;    /* column m = d - row */

    /* Exponent for the numerator: m * (2*n + 1) */
    idx_t exp = col * (2 * row + 1);

    /* Compute (2^exp + 1) / (2^col + 1) */
    seq_t num = pow2(exp);   /* 2^exp */
    num += 1;
    seq_t den = pow2(col);   /* 2^col */
    den += 1;
    return num / den;
}
