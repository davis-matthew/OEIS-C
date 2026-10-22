/*
 * A381049  Triangle T(n,k) read by rows: where T(n,k) is the number of the
 *          k-th eliminated person in the variation of the Josephus
 *          elimination process for n people, where one person is skipped and
 *          two people are eliminated.
 * https://oeis.org/A381049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   simulate_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Simulates the Josephus variation where one person is
 * skipped and the next two are eliminated, producing the triangle
 * read by rows. Returns the n‑th element of that flattened triangle. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/simulate_row.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but handled safely */

    /* Find row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    idx_t prev_cum = cum - r;          /* total terms before this row */
    idx_t pos_in_row = n - prev_cum - 1;  /* zero‑based index within the row */

    seq_t *row = simulate_row(r);      /* generate the elimination order */
    if (!row) return 0;                /* unlikely for small r, but safe */

    seq_t result = row[pos_in_row];
    free(row);
    return result;
}
