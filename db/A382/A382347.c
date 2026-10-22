/*
 * A382347  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) = [x^n] Product_{j=0..n} (1 + (k*n+j)*x).
 * https://oeis.org/A382347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the square array A(n,k) read by
 *            antidiagonals. For the n-th index we first find the diagonal d
 *            with d(d+1)/2 ≤ n < (d+1)(d+2)/2, then let pos = n - d(d+1)/2.
 *            The coordinates of the entry are (row=pos, col=d-pos), and a(n)
 *            = coefficient of x^row in ∏_{j=0..row} (1 + (col*r...
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>   /* for sqrt used only on the index */

/* Find the diagonal d such that d*(d+1)/2 <= idx < (d+1)*(d+2)/2.
   The result fits easily into size_t for all idx that appear in
   the testing range. */
static size_t find_diagonal(idx_t idx)
{
    /* Use the closed form with a small correction loop to avoid
       rounding errors of the double square root. */
    double d_est = (sqrt(8.0 * (double)idx + 1.0) - 1.0) / 2.0;
    size_t d = (size_t)d_est;

    /* Adjust upward if underestimated. */
    while ( (size_t)((__int128)(d + 1) * (d + 2) / 2) <= idx )
        d++;

    /* Adjust downward if overestimated. */
    while ( (size_t)((__int128)d * (d + 1) / 2) > idx )
        d--;

    return d;
}

seq_t a(idx_t n)
{
    /* Map the global index n to (row, col) coordinates of the array. */
    size_t d   = find_diagonal(n);
    size_t prev = (size_t)((__int128)d * (d + 1) / 2);   /* terms before diagonal d */
    size_t pos = (size_t)(n - prev);                    /* 0-based position within diagonal */

    size_t row = pos;          /* n in the formula */
    size_t col = d - pos;      /* k in the formula */

    /* Allocate coefficient array for the polynomial.
       We need indices 0 .. row+1 to allow updates up to degree row+1. */
    size_t len = row + 2;
    seq_t *coeff = (seq_t *)calloc(len, sizeof(seq_t));
    if (!coeff) {
        /* out of memory – return 0 as a safe sentinel. */
        return (seq_t)0;
    }
    coeff[0] = (seq_t)1;

    /* Multiply successive factors (1 + a_j x) with a_j = col*row + j. */
    for (size_t j = 0; j <= row; ++j) {
        seq_t a_j = (seq_t)col * (seq_t)row + (seq_t)j;
        for (size_t i = j + 1; i > 0; --i) {
            coeff[i] += a_j * coeff[i - 1];
        }
    }

    seq_t result = coeff[row];   /* coefficient of x^row */

    free(coeff);
    return result;
}
