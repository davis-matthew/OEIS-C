/*
 * A302764  Pascal-like triangle with A000012 as the left border and A080956
 *          as the right border.
 * https://oeis.org/A302764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 59, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pascal-like triangle with left border 1 and right border (n+1)*(2-n)/2;
   the sequence a(n) is obtained by reading the triangle row‑by‑row (row 0 first). */

#include <stdlib.h>

/* Computes the nth term of OEIS sequence A302764. */
seq_t a(idx_t n)
{
    /* Determine the row R (0‑based) that contains the nth element. */
    idx_t R = 0;
    while ((R + 1) * (R + 2) / 2 < n) ++R;

    /* Number of elements before row R. */
    idx_t before = R * (R + 1) / 2;
    /* Column index within row R (0‑based). */
    idx_t k = n - before - 1;

    /* Allocate two rows of length R+1. */
    seq_t *prev = (seq_t *)malloc((R + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((R + 1) * sizeof(seq_t));

    /* Row 0 */
    prev[0] = 1;

    /* Build rows 1 .. R. */
    for (idx_t r = 1; r <= R; ++r) {
        cur[0] = 1;                                 /* left border */
        for (idx_t c = 1; c < r; ++c) {               /* interior cells */
            cur[c] = prev[c] + prev[c - 1];
        }
        /* right border value for row r */
        cur[r] = ((seq_t)(r + 1) * ((seq_t)2 - (seq_t)r)) / ((seq_t)2);

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[k];          /* the required element */

    /* Free the allocated memory. */
    free(prev);
    free(cur);

    return result;
}
