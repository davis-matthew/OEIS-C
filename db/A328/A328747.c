/*
 * A328747  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) is Sum_{i=0..n} (-1)^(n-i)*binomial(n,i)*Sum_{j=0..i}
 *          binomial(i,j)^k.
 * https://oeis.org/A328747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the square array T(i,k) in antidiagonal order.
 *            T(i,k) = ∑_{j=0..i} (−1)^{i−j} * binomial(i,j) * ∑_{t=0..j}
 *            binomial(j,t)^k
 * Range:     u8 n <= 38, u16 n <= 59, u32 n <= 110, u64 n <= 180,
 *            u128 n <= 336, big any n
 * Validated: standard, pass, 82 terms through n = 5042 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: binomial(idx_t n, idx_t k) and seq_pow(seq_t base, idx_t exp). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* Determine the coordinates (row, col) of the term at index n in antidiagonal order. */
static void antidiag_coords(idx_t n, idx_t *row, idx_t *col)
{
    idx_t d = 0;
    /* Find the largest d with d(d+1)/2 <= n. */
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    /* After loop, d satisfies d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t start = d * (d + 1) / 2;
    *row = n - start;           /* row index i */
    *col = d - *row;            /* column index k */
}

seq_t a(idx_t n)
{
    idx_t i, j;
    idx_t n_rows, k;
    antidiag_coords(n, &n_rows, &k);

    seq_t pos = 0;   /* sum of terms with even sign   */
    seq_t neg = 0;   /* sum of terms with odd  sign   */

    for (i = 0; i <= n_rows; ++i) {
        seq_t bin_ni = binomial(n_rows, i);

        seq_t inner = 0;
        for (j = 0; j <= i; ++j) {
            seq_t c_ij = binomial(i, j);
            seq_t pw   = ipow_idx(c_ij, k);   /* c_ij^k */
            inner += pw;
        }

        seq_t term = bin_ni * inner;
        if ((n_rows - i) & 1)
            neg += term;
        else
            pos += term;
    }

    /* The final result is non‑negative, so pos ≥ neg.  */
    return pos - neg;
}
