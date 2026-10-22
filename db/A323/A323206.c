/*
 * A323206  A(n, k) = hypergeometric([-k, k+1], [-k-1], n), square array read
 *          by ascending antidiagonals for n,k >= 0.
 * https://oeis.org/A323206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(row,col) where (row,col) are the coordinates of index n
 *            in the square array read by ascending antidiagonals. A(row,col)
 *            is computed by: A(row,col) = Σ_{j=0..col} C(col+j, col) *
 *            (col+1-j)/(col+1) * row^j
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* helper: multiplicative binomial coefficient
   nCk = n! / (k! (n-k)!) */
static inline seq_t binom_mul(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first; division is exact */
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

/* compute (row,col) coordinates of index n in the antidiagonal layout */
static inline void antidiagonal_coords(idx_t n, idx_t *row, idx_t *col) {
    /* find smallest d such that (d+1)(d+2)/2 > n */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;
    /* cumulative terms before this diagonal: d(d+1)/2 */
    idx_t prev = d * (d + 1) / 2;
    idx_t offset = n - prev;          /* zero‑based offset in the diagonal */
    *row = d - offset;
    *col = offset;
}

/* value of A(row,col) using the hypergeometric–derived formula */
static inline seq_t a_value(idx_t row, idx_t col) {
    seq_t result = 0;
    seq_t pow = 1;                     /* row^j, j starts at 0 */
    seq_t row_t = (seq_t)row;
    for (idx_t j = 0; j <= col; ++j) {
        seq_t comb = binom_mul(col + j, col);              /* C(col+j, col) */
        seq_t num   = comb * (seq_t)(col + 1 - j);
        seq_t term  = num / (seq_t)(col + 1);              /* exact division */
        term *= pow;                                      /* multiply by row^j */
        result += term;
        pow *= row_t;                                      /* advance row^j */
    }
    return result;
}

/* external sequence function required by the build */
extern seq_t a(idx_t n) {
    idx_t row, col;
    antidiagonal_coords(n, &row, &col);
    return a_value(row, col);
}
