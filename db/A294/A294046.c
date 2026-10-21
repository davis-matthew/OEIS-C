/*
 * A294046  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f.: exp(1/(1-x)^k - 1).
 * https://oeis.org/A294046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 41, u32 n <= 98, u64 n <= 204,
 *            u128 n <= 518, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A294046 – Square array A(n,k) read by anti‑diagonals
   A(0,k) = 1
   A(n,k) = k * (n-1)! * Σ_{j=1..n} binomial(j+k-1 , k) * A(n-j,k) / (n-j)!   for n>0
   The flattened sequence a(n) is obtained by reading A(i,d-i) where
   d*(d+1)/2 + i = n, with i ≥ 0, d = ⌊(√(8n+1)-1)/2⌋. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Static storage for the computed columns of the square array */
static seq_t **col      = NULL;   /* pointers to columns */
static size_t *col_len  = NULL;   /* number of elements currently computed in each column */
static size_t   col_cnt = 0;      /* number of allocated columns */

/* Ensure that column k has entries up to (but not including) need_n.
   All function arguments are in idx_t; conversions to size_t are safe
   because need_n ≤ n ≤ 10000 in the intended use cases. */
static void ensure_col(idx_t k, idx_t need_n)
{
    /* Expand the column array if necessary */
    if ((size_t)k >= col_cnt) {
        size_t new_cnt = (size_t)k + 1;
        seq_t **new_col = (seq_t **)realloc(col, new_cnt * sizeof(seq_t *));
        size_t *new_len = (size_t *)realloc(col_len, new_cnt * sizeof(size_t));
        if (!new_col || !new_len) abort();
        for (size_t i = col_cnt; i < new_cnt; ++i) {
            new_col[i] = NULL;
            new_len[i] = 0;
        }
        col    = new_col;
        col_len = new_len;
        col_cnt = new_cnt;
    }

    /* Allocate or enlarge the column itself */
    if (col[k] == NULL) {
        col[k] = (seq_t *)calloc((size_t)need_n, sizeof(seq_t));
        if (!col[k]) abort();
    } else if (col_len[k] < (size_t)need_n) {
        col[k] = (seq_t *)realloc(col[k], (size_t)need_n * sizeof(seq_t));
        if (!col[k]) abort();
    }

    /* Compute any missing entries */
    size_t cur_len = col_len[k];
    for (idx_t n = cur_len; n < need_n; ++n) {
        if (n == 0) {
            col[k][0] = (seq_t)1;            /* A(0,k) = 1 */
        } else {
            seq_t sum = 0;
            seq_t factor = 1;                /* (n-1)! / (n-j)!  */
            for (idx_t j = 1; j <= n; ++j) {
                seq_t bin  = binomial(j + k - 1, k);
                seq_t prev = col[k][n - j];
                seq_t term = bin * prev * factor;
                sum += term;
                if (j < n) factor *= (n - j); /* next factorial ratio */
            }
            col[k][n] = (seq_t)k * sum;
        }
    }
    col_len[k] = (size_t)need_n;
}

/* Return the nth term of the flattened sequence a(n). */
seq_t a(idx_t n)
{
    /* Find diagonal d: largest d with d*(d+1)/2 ≤ n */
    idx_t d = 0;
    while (d * (d + 1) / 2 <= n)
        ++d;
    --d;                       /* now d satisfies the inequality */

    idx_t i = n - d * (d + 1) / 2;   /* row index */
    idx_t k = d - i;                 /* column index */

    /* Ensure column k has computed values up to row i */
    ensure_col(k, i + 1);

    return col[k][i];
}
