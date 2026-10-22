/*
 * A306533  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{j=1..n} floor(n/j^k).
 * https://oeis.org/A306533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=1..row} floor(row / j^col), where row and col are
 *            obtained by reading the square array A(row,col) = Sum_{j=1..row}
 *            floor(row / j^col) along antidiagonals
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the antidiagonal that contains the n‑th term. */
    idx_t d = 0;      /* diagonal number (starting at 1) */
    idx_t t = 0;      /* cumulative number of terms up to this diagonal */
    while (t < n) {
        ++d;
        t += d;      /* t = d*(d+1)/2 when loop exits */
    }

    /* Position of n within that diagonal (0‑based). */
    idx_t prev = t - d;           /* total terms before this diagonal */
    idx_t pos  = n - prev - 1;    /* 0‑based index inside the diagonal */

    /* Convert diagonal position to (row, col). */
    idx_t row = pos + 1;               /* row index (≥1)      */
    idx_t col = d - pos - 1;           /* column index (≥0)   */

    /* Compute the value A(row,col) = Σ_{j=1..row} floor(row / j^col). */
    seq_t sum = 0;
    for (idx_t j = 1; j <= row; ++j) {
        /* Compute j^col with overflow protection. */
        idx_t pow = 1;
        for (idx_t k = 0; k < col; ++k) {
            /* If pow would overflow or exceed 'row', set to a value > row. */
            if (pow > row / j) {
                pow = row + 1;   /* ensures floor(row/pow) == 0 */
                break;
            }
            pow *= j;
        }
        sum += (seq_t)row / (seq_t)pow;   /* j^col fits in idx_t */
    }
    return sum;
}
