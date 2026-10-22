/*
 * A334705  Triangle read by rows: T(n,k) (1 <= k <= n) = number of ways to
 *          choose three points from an n X k grid of points which are the
 *          vertices of a triangle of nonzero area.
 * https://oeis.org/A334705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,b) where T(r,b) counts the number of triples of
 *            distinct points in an r x b grid that form a non‑zero‑area
 *            triangle. The triangle is read by rows; r is the row number and
 *            b the column index (1 ≤ b ≤ r). T(r,b) is computed by T(r,b) =
 *            C(r*b,3) − r·C(b,3) − b·C(r,3) − 2·∑_{dx=1}^{r−1}∑_{...
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Include binomial helper */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Convert flat index n to (row, col) – rows are numbered from 1 */
    idx_t row = 1;
    idx_t tri = 1;                 /* row*(row+1)/2 */
    while (tri < n) {
        ++row;
        tri += row;
    }
    idx_t prev = (row - 1) * row / 2;
    idx_t col = n - prev;          /* 1‑based column index */

    /* Total triples in the grid */
    seq_t total = binomial(row * col, 3);

    /* Subtract horizontal and vertical collinear triples */
    seq_t horiz = (seq_t)row * binomial(col, 3);
    seq_t vert  = (seq_t)col * binomial(row, 3);

    /* Subtract other collinear triples */
    seq_t sum = 0;
    for (idx_t dx = 1; dx < row; ++dx) {
        for (idx_t dy = 1; dy < col; ++dy) {
            idx_t g = gcd_idx(dx, dy);
            if (g > 1) {
                seq_t term = (seq_t)(g - 1);
                term *= (seq_t)(row - dx);
                term *= (seq_t)(col - dy);
                sum += term;
            }
        }
    }

    seq_t result = total;
    result -= horiz;
    result -= vert;
    result -= 2 * sum;
    return result;
}
