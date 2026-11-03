/*
 * A382993  Square array A(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where A(n,k) = -(1/n) * Sum_{d|n} phi(n/d) * (-k)^d.
 * https://oeis.org/A382993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = -(1/i) * Σ_{t=1..i} (‑j)^{gcd(i,t)}, where (i,j) is the
 *            antidiagonal coordinates of n (i,j ≥ 1)
 * Range:     i8 n <= 23, i16 n <= 60, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   gcd, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ipow.h"

/* Convert the linear index n (≥ 1) to the i‑th row and j‑th column
   of the square array read by antidiagonals downwards.  The
   enumeration is
         (1,1), (1,2), (2,1), (1,3), (2,2), (3,1), … */
static void
idx_to_ij(idx_t n, idx_t *row, idx_t *col)
{
    /* Find the diagonal d such that d*(d+1)/2 ≥ n.  */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;

    idx_t prev = (d - 1) * d / 2;          /* elements before this diagonal */
    idx_t offset = n - prev - 1;           /* 0‑based position on the diagonal */

    *row = offset + 1;                     /* i ≥ 1 */
    *col = d - offset;                     /* j ≥ 1 */
}

/* Compute A(i,j) = -(1/i) * Σ_{t=1..i} (‑j)^{gcd(i,t)}.  */
static seq_t
compute_A_row_col(idx_t i, idx_t j)
{
    seq_t sum = 0;

    for (idx_t t = 1; t <= i; ++t) {
        seq_t g = gcd((seq_t)i, (seq_t)t);          /* gcd(i,t)  */
        seq_t base = (seq_t)(-(int64_t)j);           /* –j       */
        seq_t term = ipow(base, g);              /* (‑j)^g   */
        sum += term;
    }

    /* The division is exact by definition of A.  */
    seq_t res = -(sum / (seq_t)i);
    return res;
}

seq_t a(idx_t n)
{
    idx_t row, col;
    idx_to_ij(n, &row, &col);
    return compute_A_row_col(row, col);
}
