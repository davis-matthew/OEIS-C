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
 * Helpers:   big_pow, gcd, idx_to_ij
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include "../utils/idx_to_ij.h"

/* Compute A(i,j) = -(1/i) * Σ_{t=1..i} (‑j)^{gcd(i,t)}.  */
static seq_t
compute_A_row_col(idx_t i, idx_t j)
{
    seq_t sum = 0;

    for (idx_t t = 1; t <= i; ++t) {
        seq_t g = gcd((seq_t)i, (seq_t)t);          /* gcd(i,t)  */
        seq_t base = (seq_t)(-(int64_t)j);           /* –j       */
        seq_t term = big_pow(base, g);              /* (‑j)^g   */
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
