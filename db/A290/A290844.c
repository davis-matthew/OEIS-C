/*
 * A290844  Square array read by antidiagonals downwards: A(n, k) =
 *          (Sum_{i=1..n} i^k) - (n+1)^k for n >= 1, k >= 1.
 * https://oeis.org/A290844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (∑_{i=1}^{x} i^y) – (x+1)^y, where (x,y) are the * row
 *            and column of the square array A(n,k) read along antidiagonals *
 *            downwards (OEIS A290844)
 * Range:     i8 n <= 16, i16 n <= 47, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal d such that T(d-1) < n ≤ T(d), where
     * T(d) = d*(d+1)/2 is the d‑th triangular number. */
    idx_t d   = 1;          /* current antidiagonal index */
    idx_t tri = 1;          /* T(d) */
    while (n > tri) {
        ++d;
        tri += d;          /* now tri = T(d) */
    }

    /* Determine position (row, column) within this diagonal.
     * offset counts how far from the start of the diagonal:
     *   offset = 0 → row=1, column=d
     *   offset = d‑1 → row=d, column=1. */
    idx_t prev_tri = tri - d;          /* T(d–1) */
    idx_t offset   = n - prev_tri - 1; /* 0‑based offset inside diagonal */

    idx_t row = offset + 1;            /* i in A(i,j) */
    idx_t col = d - offset;            /* j in A(i,j) */

    /* Compute the required sum:   Σ_{t=1..row} t^col  –  (row+1)^col */
    seq_t sum = 0;
    for (idx_t t = 1; t <= row; ++t) {
        sum += big_pow((seq_t)t, (seq_t)col);
    }
    sum -= big_pow((seq_t)(row + 1), (seq_t)col);
    return sum;
}
