/*
 * A306913  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals, where
 *          column k is the expansion of g.f. 1/((1+x)^k+x^k).
 * https://oeis.org/A306913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is read by antidiagonals from the square array A(n,k) =
 *            (-1)^n * Σ_{j=0..⌊n/k⌋} (-1)^(((k+1) mod 2)·j) * binomial(n+k-1,
 *            k·j+k-1)
 * Range:     i8 n <= 38, i16 n <= 151, i32 n <= 559, i64 n <= 2143,
 *            i128 n <= 8351, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that T_{d} > n >= T_{d-1}, where T_d = d*(d+1)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Previous cumulative count of elements */
    idx_t prev = d * (d + 1) / 2;

    /* Offset within the diagonal: row index */
    idx_t row = n - prev;              /* row index i */
    idx_t col = d + 1 - row;           /* column index k (>=1) */

    seq_t val = 0;

    /* Sum over l = 0 .. floor(row/col) */
    idx_t maxl = row / col;
    for (idx_t l = 0; l <= maxl; ++l)
    {
        seq_t term = binomial(row + col - 1, col * l + col - 1);

        /* Apply factor (-1)^(((k+1) mod 2) * l) */
        if ((col % 2) == 0 && (l & 1))
            term = -term;

        val += term;
    }

    /* Multiply by (-1)^row */
    if (row & 1)
        val = -val;

    return val;
}
