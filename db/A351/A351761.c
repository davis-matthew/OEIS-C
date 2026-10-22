/*
 * A351761  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = n! * Sum_{j=0..n} k^(n-j) * (n-j)^j/j!.
 * https://oeis.org/A351761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of n
 *            * (i = offset along the d‑th diagonal, j = d - offset). *
 *            Recurrence for the square array: * T(0,k) = 1 * T(n,k) = k * n *
 *            Σ_{j=0}^{n-1} binom(n-1,j) * T(j,k) (n > 0) * * The
 *            implementation computes the coordinates of the n‑th term o...
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* maximum diagonal needed for n up to at least 10,000; set generously */
#define MAX_D 2000

/* Multiplicative binomial coefficient for small indices (idx_t). */
static seq_t
small_binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;      /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * ((seq_t)(a - b + i));
        res = res / ((seq_t)i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* ---------- find the antidiagonal and coordinates ---------- */
    idx_t d = 0, cum = 0;
    while (cum + d + 1 <= n) {
        cum += d + 1;
        ++d;
    }
    idx_t offset = n - cum;
    idx_t i = offset;            /* row index (0‑based)   */
    idx_t k = d - offset;        /* column index (k>=0)   */

    /* ---------- evaluate T(i,k) via the recurrence ------------- */
    if (i == 0)
        return 1;                /* T(0,k) = 1 */

    /* stack array for intermediate values; size fits any diagonal
       needed for n <= 10,000 (d <= 141).  The constant 2000 is more
       than sufficient for the typical test range. */
    seq_t t[MAX_D];

    t[0] = 1;                    /* T(0,k) */
    for (idx_t row = 1; row <= i; ++row) {
        seq_t sum = 0;
        for (idx_t j = 0; j < row; ++j) {
            seq_t bin = small_binom(row - 1, j);
            sum += bin * t[j];
        }
        t[row] = (((seq_t)k) * ((seq_t)row)) * sum;
    }

    return t[i];                 /* T(i,k) = a(n) */
}
