/*
 * A244131  Triangle read by rows: terms T(n,k) of a binomial decomposition of
 *          n as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Triangle entry T(row, col) read by rows. T(row, col) =
 *            (-col)^(col-1) * (1+col)^(row-col) * C(row, col) for col > 0,
 *            T(row,0) = 0. Here n is the global index (starting at 0)
 * Range:     i8 n <= 12, i16 n <= 30, i32 n <= 60, i64 n <= 145,
 *            i128 n <= 393, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t g)
{
    /* Find the row r so that the sum of lengths of rows 0..(r-1) ≤ g < sum of lengths 0..r. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= g)  /* row r ends at index S(r)-1 = (r+1)(r+2)/2-1 */
        ++r;

    /* Index of first element of this row */
    idx_t row_start = r * (r + 1) / 2;
    /* Local column index within the row */
    idx_t k = g - row_start;

    /* T(row,0) is defined to be 0 */
    if (k == 0)
        return 0;

    /* Compute binomial coefficient C(r,k) */
    seq_t bin = binomial(r, k);

    /* Compute k^(k-1) */
    seq_t powk = big_pow((seq_t)k, (seq_t)(k - 1));

    /* Compute (k+1)^(r-k) */
    seq_t powk1 = big_pow((seq_t)(k + 1), (seq_t)(r - k));

    /* T(r,k) = (-1)^(k-1) * k^(k-1) * (k+1)^(r-k) * C(r,k) */
    seq_t term = bin * powk;
    term = term * powk1;

    if ((k % 2) == 0)   /* even k → negative sign */
        term = -term;

    return term;
}
