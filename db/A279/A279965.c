/*
 * A279965  Square array read by antidiagonals upwards in which each term is
 *          the number of prior elements in the same row, column, diagonal, or
 *          antidiagonal whose parity is the same as the parity of n.
 * https://oeis.org/A279965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the number of prior terms m<n that are in the
   same row, same column, same main diagonal (i-j), or same antidiagonal
   (i+j) as the element at index n, and whose parity of value equals the
   parity of n.  The array is read by antidiagonals upwards.  The mapping
   from index n to coordinates (row, col) is:
      let s be the smallest integer such that (s-1)*s/2 >= n;
      let offset = n - (s-2)*(s-1)/2 - 1;
      row = (s-1) - offset;  col = offset + 1. */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* global state */
static seq_t *A      = NULL;   /* a(n), 1‑based, index 0 unused */
static idx_t *row_   = NULL;   /* row numbers */
static idx_t *col_   = NULL;   /* column numbers */
static idx_t  max_n  = 0;      /* largest n computed */

/* convert index n to row and column */
static inline void index_to_coords(idx_t n, idx_t *row, idx_t *col)
{
    /* compute s ≈ ceil((1+sqrt(1+8n))/2) */
    long double nd = (long double)n;
    idx_t s = (idx_t)((1.0L + sqrtl(1.0L + 8.0L * nd)) / 2.0L);
    /* adjust to satisfy (s-1)*s/2 >= n */
    while (((__uint128_t)(s - 1) * s) / 2 < (unsigned __int128)n) ++s;
    __uint128_t prev = ((__uint128_t)(s - 2) * (s - 1)) / 2;
    idx_t offset   = (idx_t)((unsigned __int128)n - prev - 1);
    *row = (s - 1) - offset;
    *col = offset + 1;
}

/* main function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑positive input */

    if (n > max_n) {
        size_t new_cap = (size_t)n + 1;   /* 1‑based indexing, 0 unused */
        if (new_cap > (size_t)max_n) {
            A   = (seq_t *)realloc(A,   new_cap * sizeof(seq_t));
            row_ = (idx_t *)realloc(row_, sizeof(idx_t) * new_cap);
            col_ = (idx_t *)realloc(col_, sizeof(idx_t) * new_cap);
        }

        for (idx_t k = max_n + 1; k <= n; ++k) {
            idx_t i, j;
            index_to_coords(k, &i, &j);
            int parity = (int)(k & 1);   /* parity of index k */

            seq_t val = 0;
            for (idx_t m = 1; m < k; ++m) {
                if (((int)(A[m] & 1)) != parity) continue;

                if (row_[m] == i) { val++; continue; }

                if (col_[m] == j) { val++; continue; }

                if ((row_[m] + col_[m]) == (i + j)) { val++; continue; }

                if ((int64_t)row_[m] - (int64_t)col_[m] ==
                    (int64_t)i - (int64_t)j) { val++; continue; }
            }

            A[k]   = val;
            row_[k] = i;
            col_[k] = j;
        }
        max_n = n;
    }
    return A[n];
}
