/*
 * A288842  Triangle (sans apex) of coefficients of terms of the form
 *          (eM_1)^j*(eM_2)^k re construction of triangle A287768.
 * https://oeis.org/A288842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A288842.  Triangle T(r,k) defined by
   T(1,1)=1, T(1,2)=2 and for r≥2, k≥1,
      T(r,k) = 3*T(r-1,k-1) + 3*T(r-1,k)  (with T(r-1,0)=0, T(r-1,r)=0).
   The sequence a(n) is the row‑wise reading of this triangle:
   a(1)=1, a(2)=2, a(3)=3, a(4)=9, a(5)=6, …  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine row and column of the n‑th element */
    idx_t total = 0;
    idx_t row   = 1;
    while (1) {
        idx_t len = row + 1;            /* row r has r+1 entries */
        if (total + len >= n)
            break;
        total += len;
        row++;
    }
    idx_t col = n - total;              /* 1‑based column index */

    /* Base row T(1,·) */
    idx_t  prev_len = 2;
    seq_t *prev = (seq_t*)malloc(prev_len * sizeof(seq_t));
    if (!prev) return (seq_t)0;          /* allocation failure fall‑through */
    prev[0] = (seq_t)1;
    prev[1] = (seq_t)2;

    if (row == 1) {
        seq_t result = prev[col-1];
        free(prev);
        return result;
    }

    /* Build successive rows up to the required one */
    for (idx_t r = 2; r <= row; ++r) {
        idx_t cur_len = r + 1;
        seq_t *cur = (seq_t*)malloc(cur_len * sizeof(seq_t));
        if (!cur) { free(prev); return (seq_t)0; }

        /* First element of the new row */
        cur[0] = (seq_t)3 * prev[0];

        /* Middle elements */
        for (idx_t j = 1; j < cur_len - 1; ++j)
            cur[j] = (seq_t)3 * prev[j-1] + (seq_t)3 * prev[j];

        /* Last element of the new row */
        cur[cur_len - 1] = (seq_t)3 * prev[cur_len - 2];

        free(prev);
        prev = cur;
    }

    seq_t result = prev[col-1];
    free(prev);
    return result;
}
