/*
 * A293202  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of g.f. Product_{i>0} Sum_{j=0..k}
 *          j!*x^(j*i).
 * https://oeis.org/A293202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 179, u32 n <= 363, u64 n <= 923,
 *            u128 n <= 2519, big any n
 * Helpers:   antidiag_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A293202: a(n) = entry (row,col) of the square array read by antidiagonals.
   The array A(row,col) is the coefficient of x^row in
   ∏_{m>0} Σ_{j=0..col} j! · x^(j·m).
   The mapping from index n to (row,col) is:
        t   = ⌊(√(8n+1)-1)/2⌋
        row = n - t(t+1)/2
        col = t - row
   (row+col = t).  This computates the sequence exactly for every n ≥ 0.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/antidiag_t.h"

/* Main sequence routine */
seq_t a(idx_t n)
{
    /* Convert n to (row,col) coordinates on the array. */
    idx_t t   = antidiag_t(n);
    idx_t row = n - (t * (t + 1)) / 2;
    idx_t col = t - row;
    if (row == 0) return (seq_t)1;   /* A(0, k) = 1 for all k */
    if (col == 0) return (seq_t)0;   /* A(n,0) = 0 for n>0 */

    /* Allocate factorial table up to col. */
    seq_t *fact = NULL;
    fact = (seq_t*)calloc((size_t)col + 1, sizeof(seq_t));
    if (!fact) return (seq_t)0;
    fact[0] = (seq_t)1;
    for (idx_t j = 1; j <= col; ++j)
        fact[j] = fact[j - 1] * (seq_t)j;

    /* DP arrays for current and previous rows. */
    size_t len = (size_t)row + 1;
    seq_t *dp_prev = NULL;
    seq_t *dp_curr = NULL;
    dp_prev = (seq_t*)calloc(len, sizeof(seq_t));
    dp_curr = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp_prev || !dp_curr) {
        free(fact);
        free(dp_prev);
        free(dp_curr);
        return (seq_t)0;
    }
    dp_prev[0] = (seq_t)1;  /* base: empty sum */

    /* Iterate over part sizes m = 1 .. row. */
    for (idx_t m = 1; m <= row; ++m) {
        /* Copy dp_prev to dp_curr for j=0 case. */
        for (size_t s = 0; s < len; ++s)
            dp_curr[s] = dp_prev[s];

        /* Add contributions for j = 1 .. col, j*m <= row. */
        for (idx_t j = 1; j <= col; ++j) {
            size_t shift = (size_t)m * (size_t)j;
            if (shift > row) break;
            seq_t w = fact[j];
            for (size_t s = shift; s <= row; ++s)
                dp_curr[s] += dp_prev[s - shift] * w;
        }

        /* Swap pointers for next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    seq_t result = dp_prev[row];

    /* Clean up */
    free(dp_prev);
    free(dp_curr);
    free(fact);
    return result;
}
