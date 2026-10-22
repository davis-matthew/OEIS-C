/*
 * A393631  Triangle read by rows giving successive states of the Code 10 1-D
 *          cellular automaton when started from a single ON cell.
 * https://oeis.org/A393631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 8226, u16 n <= 8226, u32 n <= 8226, u64 n <= 8226,
 *            u128 n <= 8226, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is obtained by reading a 1‑D cellular automaton
   (Code 10) in row‑major order, starting from a single ON cell.
   Row r has length 4·r + 1.  To compute the next row we pad the
   previous row with two dead cells on each side, then for each cell
   count the number of ON cells in the 5‑cell neighborhood
   (including the two cells to the left and right).  The new cell
   is ON iff the count is 1 or 3.  Initial row: 1.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Global storage for computed rows */
static seq_t **ca_rows = NULL;
static size_t ca_rows_cap = 0;

/* Ensure there is room for at least 'need' row pointers */
static void
ensure_rows_capacity(size_t need)
{
    if (ca_rows_cap < need) {
        size_t new_cap = ca_rows_cap ? ca_rows_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        seq_t **new_arr = (seq_t **)realloc(ca_rows, new_cap * sizeof(seq_t *));
        if (!new_arr) exit(EXIT_FAILURE);
        ca_rows = new_arr;
        ca_rows_cap = new_cap;
    }
}

/* Recursively compute row 'r' (and all lower rows if missing) */
static void
compute_row(idx_t r)
{
    /* The caller guarantees that ca_rows_cap ≥ r+1 */
    if (r < (idx_t)ca_rows_cap && ca_rows[(size_t)r]) return;

    /* Base case: first row */
    if (r == 0) {
        size_t len = 1;
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);
        row[0] = 1;
        ca_rows[0] = row;
        return;
    }

    /* Ensure lower rows are available */
    compute_row(r - 1);

    /* Compute this row */
    size_t len = 4 * (size_t)r + 1;            /* current row length   */
    seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
    if (!row) exit(EXIT_FAILURE);

    seq_t *prev    = ca_rows[(size_t)r - 1];
    size_t prev_len = 4 * ((size_t)r - 1) + 1; /* previous row length */

    for (size_t j = 0; j < len; ++j) {
        /* Neighborhood in the padded previous row corresponds to
           previous indices k from max(0, j-4) to min(prev_len-1, j)   */
        int count = 0;
        int k_start = (int)j - 4;
        if (k_start < 0) k_start = 0;
        int k_end = (int)j;
        if (k_end > (int)prev_len - 1) k_end = (int)prev_len - 1;
        for (int k = k_start; k <= k_end; ++k)
            count += (int)prev[(size_t)k];
        row[j] = (count == 1 || count == 3) ? 1 : 0;
    }

    ca_rows[(size_t)r] = row;
}

seq_t a(idx_t n)
{
    /* Find the row that contains index n.
       Total elements up to the end of row r: (r+1)(2r+1) */
    idx_t r = 0;
    while ((r + 1) * (2 * r + 1) <= n)
        ++r;

    /* Make sure we can store row r */
    ensure_rows_capacity((size_t)r + 1);
    if (!ca_rows[(size_t)r])
        compute_row(r);

    /* Compute offset within the row */
    size_t offset;
    if (r == 0) {
        offset = 0;
    } else {
        /* Elements before row r: r(2r-1) */
        offset = (size_t)n - (size_t)r * (2 * (size_t)r - 1);
    }

    return ca_rows[(size_t)r][offset];
}
