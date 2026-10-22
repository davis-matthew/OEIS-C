/*
 * A358623  Regular triangle read by rows. T(n, k) = {{n, k}}, where {{n, k}}
 *          are the second order Stirling set numbers (or second order
 *          Stirling numbers). T(n, k) for 0 <= k <= n.
 * https://oeis.org/A358623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   idx_linear
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for T(n,k):
   T(0,0)=1,  T(n,0)=0  for n>0,
   T(n,k)=(n-1)*T(n-2,k-1)+k*T(n-1,k)   for 0<k<=n.
   The sequence a(n) is obtained by reading the triangle T in row‑wise
   order: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, …  (see OEIS A358623). */

#include <stdlib.h>
#include <string.h>
#include "../utils/idx_linear.h"

/* Flat storage for the triangular array T. */
static seq_t *T_table   = NULL;          /* array of all T(n,k) entries */
static idx_t  T_max_row = (idx_t)-1;     /* last row that has been filled */

/* Ensure that rows 0..r_needed are computed and stored in T_table. */
static void ensure_capacity(idx_t r_needed)
{
    if (T_table != NULL && r_needed <= T_max_row)
        return;          /* already done */

    /* New total number of elements needed. */
    size_t new_size = (size_t)((r_needed + 1) * (r_needed + 2) / 2);

    /* Allocate a new buffer. */
    seq_t *new_table = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_table)
        abort();        /* out of memory */

    /* If we had a previous table, copy its contents. */
    if (T_table != NULL) {
        memcpy(new_table, T_table,
               (T_max_row + 1) * (T_max_row + 2) / 2 * sizeof(seq_t));
        free(T_table);
    }

    /* Compute missing rows (starting at the first not yet computed). */
    idx_t start_row = (T_table == NULL) ? 0 : T_max_row + 1;
    for (idx_t i = start_row; i <= r_needed; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            seq_t val;
            if (j == 0) {
                if (i == 0)
                    val = (seq_t)1;      /* T(0,0) */
                else
                    val = (seq_t)0;      /* T(n,0)=0 for n>0 */
            } else {
                seq_t term1 = (seq_t)0;
                if (i >= 2) {
                    size_t idx1 = idx_linear(i - 2, j - 1);
                    term1 = (seq_t)(i - 1) * new_table[idx1];
                }
                seq_t term2 = (seq_t)0;
                if (j <= i - 1) {
                    size_t idx2 = idx_linear(i - 1, j);
                    term2 = (seq_t)j * new_table[idx2];
                }
                val = term1 + term2;
            }
            new_table[idx_linear(i, j)] = val;
        }
    }

    T_table   = new_table;
    T_max_row = r_needed;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t c = n - r * (r + 1) / 2;

    ensure_capacity(r);
    return T_table[idx_linear(r, c)];
}
