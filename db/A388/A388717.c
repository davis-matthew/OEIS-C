/*
 * A388717  Triangle read by rows: T(n,k) is the total number of runs of 1's
 *          in all length n binary words in which 1's occur in runs of at
 *          least k.
 * https://oeis.org/A388717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 435, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A388717: Triangle read by rows.
    T(n,k) = Σ_{t=1}^{⌊(n+1)/(k+1)⌋} t * binomial(n - t*(k-1) + 1, 2t)
    a(n) is the sequence obtained by reading the triangle row‑wise.
    The function computes a(n) for any n ≥ 1. */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; undefined otherwise */

    /* --- find the row and column of the index n in the triangular array --- */
    idx_t r = 1;                   /* current row   */
    idx_t tri = 1;                 /* r*(r+1)/2     */
    while (tri < n) {              /* linear search; r <= ~142 for n ≤ 10000 */
        ++r;
        tri = r * (r + 1) / 2;
    }
    idx_t row = r;
    idx_t prev = (row - 1) * row / 2;
    idx_t col = n - prev;          /* 1‑based column within the row */

    /* --- compute T(row, col) by the closed formula --- */
    seq_t sum = 0;
    idx_t max_t = (row + 1) / (col + 1);   /* floor((row+1)/(col+1)) */
    for (idx_t t = 1; t <= max_t; ++t) {
        idx_t n1 = row - t * (col - 1) + 1; /* n - t*(k-1) + 1 */
        idx_t k1 = 2 * t;                  /* 2*t           */
        seq_t term = binomial(n1, k1);
        term *= (seq_t)t;                  /* weight by t   */
        sum += term;
    }
    return sum;
}
