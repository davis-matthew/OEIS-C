/*
 * A279967  Square array read by antidiagonals upwards in which each term is
 *          the sum of prior elements in the same row, column, diagonal, or
 *          antidiagonal that divide n; the array is seeded with an initial
 *          value a(1)=1.
 * https://oeis.org/A279967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 251, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279967:
   a(1) = 1.
   For n > 1, a(n) equals the sum of values of all previously
   computed elements that lie in the same row, column, main diagonal,
   or anti‑diagonal as the position of n and whose value divides n.
   The array is read by antidiagonals upwards:
   the nth position has coordinates (i,j) satisfying
   n = ((i+j-1)*(i+j-2))/2 + i. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>   /* for UINT64_MAX */

/* Helper: convert index n into its antidiagonal coordinates (i,j).
   Returns 1‑based i and j. */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t s = 2;         /* sum i+j for the current diagonal */
    idx_t cum = 0;       /* number of terms before diagonal s */

    while (1) {
        idx_t len = s - 1;              /* number of terms on diagonal s */
        if (n <= cum + len) {
            *i = n - cum;              /* position inside diagonal */
            *j = s - *i;
            return;
        }
        cum += len;
        ++s;
    }
}

/* Static storage for memoised terms and their coordinates. */
static seq_t  *values = NULL;   /* a[k] */
static idx_t  *rows   = NULL;   /* i_k  */
static idx_t  *cols   = NULL;   /* j_k  */
static idx_t   computed_max = 0;

/* Ensure all terms up to 'limit' are computed and stored. */
static void compute_upto(idx_t limit)
{
    if (limit <= computed_max)
        return;                                /* already available */

    idx_t new_cap = limit + 1;                 /* allow indices 1..limit */

    values = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
    rows   = (idx_t*)realloc(rows,   new_cap * sizeof(idx_t));
    cols   = (idx_t*)realloc(cols,   new_cap * sizeof(idx_t));
    if (!values || !rows || !cols)
        exit(1);                               /* memory allocation failure */

    /* Initial seed for a(1) if not already present. */
    if (computed_max == 0) {
        values[1] = (seq_t)1;
        antidiagonal_coords(1, &rows[1], &cols[1]);   /* (1,1) */
        computed_max = 1;
    }

    /* Compute remaining terms sequentially. */
    for (idx_t n = computed_max + 1; n <= limit; ++n) {
        idx_t i_cur, j_cur;
        antidiagonal_coords(n, &i_cur, &j_cur);
        int  diag_main_cur = (int)(i_cur - j_cur);    /* i - j (can be negative) */
        idx_t diag_anti_cur = i_cur + j_cur;          /* i + j */

        seq_t sum = 0;

        for (idx_t k = 1; k < n; ++k) {
            /* Check whether (k) lies in the same row, column, main diagonal,
               or anti‑diagonal as (n).  If none, skip. */
            if (rows[k] != i_cur &&
                cols[k] != j_cur &&
                (int)(rows[k] - cols[k]) != diag_main_cur &&
                (rows[k] + cols[k]) != diag_anti_cur)
                continue;

            seq_t v = values[k];
            if (v == 0)
                continue;                           /* zero contributes nothing */

            /* A small optimisation: if v exceeds UINT64_MAX or n,
               it cannot divide n.  Checked via conversions. */
            if (v <= (seq_t)UINT64_MAX) {
                idx_t vk = (idx_t)v;
                if (vk <= n && n % vk == 0)
                    sum += v;
            }
        }

        values[n] = sum;
        rows[n] = i_cur;
        cols[n] = j_cur;
    }

    computed_max = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, but safe fallback */

    compute_upto(n);
    return values[n];
}
