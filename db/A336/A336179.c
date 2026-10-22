/*
 * A336179  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = Sum_{j=0..n} (-k)^j * binomial(n,j)^3.
 * https://oeis.org/A336179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Sum_{j=0..row} (-col)^j * binomial(row,j)^3 * where (row,
 *            col) are obtained by reading the square array T(row,col) * with
 *            T(row,col) = Sum_{j=0..row} (-col)^j * binomial(row,j)^3 * by
 *            anti‑diagonals
 * Range:     i8 n <= 23, i16 n <= 50, i32 n <= 128, i64 n <= 317,
 *            i128 n <= 849, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the mapping:
 *   n  →  (s, r, c)  where
 *     s = floor(( sqrt(8*n+1) - 1 ) / 2)   (largest s with s*(s+1)/2 ≤ n)
 *     r = n - s*(s+1)/2                     (row index)
 *     c = s - r                             (column index)
 *   a(n) = T(r,c)
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find diagonal index s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((s + 1ULL) * (s + 2ULL) / 2ULL <= n)
        ++s;

    idx_t t_s = s * (s + 1ULL) / 2ULL;        /* t_s = s*(s+1)/2   */
    idx_t row = n - t_s;                     /* row index         */
    idx_t col = s - row;                     /* column index      */

    /* Compute T(row, col) = Σ_{j=0..row} (-col)^j * binomial(row,j)^3  */
    seq_t sum = 0;
    seq_t pow_col = 1;                       /* col^j          */
    bool sign = true;                        /* true for +, false for -  */
    seq_t binom = 1;                         /* binomial(row,0)  */

    for (idx_t j = 0; j <= row; ++j) {
        /* Compute term = sign * pow_col * binom^3  */
        seq_t term = pow_col * (binom * binom * binom);
        if (!sign)
            term = -term;
        sum += term;

        /* Prepare for next j */
        if (j < row) {
            /* Update binomial: binom(row,j+1) = binom(row,j) * (row-j)/(j+1) */
            binom = binom * (seq_t)(row - j) / (seq_t)(j + 1);
            /* Update power of -col: multiply by -col.  Use a sign flag. */
            pow_col = pow_col * (seq_t)col;
            sign = !sign;
        }
    }
    return sum;
}
