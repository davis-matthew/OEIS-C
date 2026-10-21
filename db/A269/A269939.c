/*
 * A269939  Triangle read by rows, Ward numbers T(n, k) = Sum_{m=0..k} (-1)^(m
 *          + k) * binomial(n + k, n + m) * Stirling2(n + m, m), for n >= 0, 0
 *          <= k <= n.
 * https://oeis.org/A269939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 72, u64 n <= 163,
 *            u128 n <= 424, big any n
 * Helpers:   new_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r,c) where r and c are the row and column
 * of the flattened triangle, and T satisfies the recurrence
 *   T(0,0) = 1,
 *   T(n,0) = 0          for n > 0,
 *   T(n,k) = k*T(n-1,k) + (n+k-1)*T(n-1,k-1)   for n > 0 and 1 ≤ k ≤ n.
 * The value returned is a(n) = T(r,c) with r and c determined by
 * the position of n in the row‑major enumeration of the triangle.
 */

#include <stdlib.h>
#include "../utils/new_row.h"

static seq_t **rows = NULL;      /* array of pointers to each row */
static idx_t   max_row = 0;       /* highest row that has been computed */
static idx_t   rows_cap = 0;      /* capacity of the rows array */

/* Compute rows up to ‘target’ if necessary */
static void ensure_rows(idx_t target)
{
    /* initialise row storage if required */
    if (!rows) {
        rows_cap = 1;
        rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        rows[0] = new_row(1);
        rows[0][0] = (seq_t)1;   /* T(0,0) = 1 */
        max_row = 0;
    }

    /* grow the rows pointer array if needed */
    if (rows_cap <= target) {
        idx_t new_cap = rows_cap * 2;
        if (new_cap <= target) new_cap = target + 1;
        rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        rows_cap = new_cap;
    }

    /* compute missing rows up to target */
    for (idx_t i = max_row + 1; i <= target; ++i) {
        idx_t len = i + 1;
        seq_t *row = new_row(len);

        /* first element is 0 for n > 0 */
        if (i != 0)
            row[0] = (seq_t)0;
        else
            row[0] = (seq_t)1;      /* guard, though row 0 already set */

        /* compute remaining elements using recurrence */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term1 = (k <= i - 1) ? (k * rows[i - 1][k]) : (seq_t)0;
            seq_t term2 = ((i + k - 1) * rows[i - 1][k - 1]);
            row[k] = term1 + term2;
        }

        rows[i] = row;
    }

    max_row = target;
}

/* Main function requested by the problem statement */
seq_t a(idx_t n)
{
    /* determine row (r) and column (c) for the index n */
    idx_t r = 0;
    idx_t cumulative = 0;            /* number of elements before row r */
    while (cumulative + r + 1 <= n) {
        cumulative += r + 1;
        ++r;
    }
    idx_t c = n - cumulative;        /* column in row r */

    /* compute all rows up to r */
    ensure_rows(r);

    return rows[r][c];
}
