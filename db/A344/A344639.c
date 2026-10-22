/*
 * A344639  Array read by ascending antidiagonals: A(n, k) is the number of
 *          (n, k)-poly-Cauchy permutations.
 * https://oeis.org/A344639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where (i,j) are obtained by reading the infinite
 *            table A(n,k) (poly‑Cauchy permutations) along the ascending
 *            antidiagonals. The table satisfies A(0,j) = 1 A(i,0) = i! (i >
 *            0) A(i,j) = (i‑1)*A(i‑1,j) + Σ_{t=0..j} C(j,t)*A(i‑1,j‑t) (for
 *            i>0, j>0). This implementation memoises the whol...
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

static seq_t *tbl = NULL;   /* flattened table, size = tbl_size × tbl_size */
static idx_t  tbl_size = 0; /* current number of rows/columns stored */

/* Returns the n-th term of A344639 (0‑based).  No side effects other
   than memoisation. */
seq_t a(idx_t n)
{
    /* Find the diagonal d that contains n.  d satisfies
       d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    /* Grow the table if needed.  We recompute the entire table when
       a larger diagonal is required; this is inexpensive for the
       typical sizes (d ≤ 200 for n ≤ 10 000). */
    if (tbl_size <= d) {
        idx_t new_size = d + 1;                    /* new dimension */
        size_t cells   = (size_t)new_size * new_size;
        seq_t *new_tbl = (seq_t *)malloc(cells * sizeof(seq_t));
        if (!new_tbl) return (seq_t)0;             /* allocation failure */

        /* Pre‑compute factorials up to new_size‑1 to avoid repeated
           expensive calls.  seq_t can be big‑int so multiplication is
           exact. */
        seq_t *fact = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!fact) { free(new_tbl); return (seq_t)0; }
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i < new_size; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;

        /* Fill the table row by row.  Entries are computed in ascending
           i and j, guaranteeing that every required predecessor has
           already been calculated. */
        for (idx_t i = 0; i < new_size; ++i) {
            for (idx_t j = 0; j < new_size; ++j) {
                seq_t *p = new_tbl + i * new_size + j;
                if (i == 0) {
                    *p = (seq_t)1;             /* A(0,j) = 1 */
                } else if (j == 0) {
                    *p = fact[i];              /* A(i,0) = i! */
                } else {
                    /* (i-1) * A(i-1,j) */
                    seq_t part1 = (seq_t)(i - 1) * new_tbl[(i - 1) * new_size + j];
                    /* Σ_{t=0..j} C(j,t) * A(i-1, j-t) */
                    seq_t sum = 0;
                    for (idx_t t = 0; t <= j; ++t) {
                        seq_t term = binomial(j, t);
                        term = term * new_tbl[(i - 1) * new_size + (j - t)];
                        sum += term;
                    }
                    *p = part1 + sum;
                }
            }
        }

        free(fact);
        free(tbl);          /* free old table if any */
        tbl = new_tbl;
        tbl_size = new_size;
    }

    /* Translate n to (i,j) on diagonal d. */
    idx_t prev = d * (d + 1) / 2;   /* number of entries before diag d */
    idx_t t    = n - prev;          /* offset within the diagonal */
    idx_t i    = d - t;             /* row index      */
    idx_t j    = t;                 /* column index   */

    return tbl[i * tbl_size + j];
}
