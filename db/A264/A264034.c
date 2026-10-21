/*
 * A264034  Triangle read by rows: T(n,k) (n>=0, 0<=k<=A161680(n)) is the
 *          number of integer partitions of n with weighted sum k.
 * https://oeis.org/A264034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9250, u16 n <= 9482, u32 n <= 9482, u64 n <= 9482,
 *            u128 n <= 9482, big any n
 * Helpers:   gen_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A264034: a(n) is the (n+1)‑st element of the linearization of the
 * triangle T(n,k) where T(n,k) is the number of integer partitions of n
 * with weighted sum k (first part weight 0).  The sequence is generated
 * by enumerating all partitions of each n. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gen_part.h"

/* Types idx_t and seq_t are provided by the build environment. */

static seq_t **rows_data = NULL;      /* arrays of row values            */
static size_t *row_lengths = NULL;    /* size of each row                */
static idx_t  *cum_lengths = NULL;    /* cumulative counts up to row i   */
static size_t  row_count = 0;         /* number of rows stored           */

/* Build row 'n' (0‑based).  Allocate storage and fill it via recursion. */
static void build_row(idx_t n)
{
    idx_t max_w = n * (n - 1) / 2;           /* maximum weighted sum */
    size_t len = (size_t)max_w + 1;          /* number of entries in the row */
    seq_t *table = (seq_t *)calloc(len, sizeof(seq_t));
    if (!table) abort();                     /* memory exhaustion */

    gen_part(n, n, 0, 0, table);             /* fill the table */

    /* grow arrays that hold rows and metadata */
    rows_data = (seq_t **)realloc(rows_data, (row_count + 1) * sizeof(seq_t *));
    if (!rows_data) abort();
    rows_data[row_count] = table;

    row_lengths = (size_t *)realloc(row_lengths, (row_count + 1) * sizeof(size_t));
    if (!row_lengths) abort();
    row_lengths[row_count] = len;

    cum_lengths = (idx_t *)realloc(cum_lengths, (row_count + 1) * sizeof(idx_t));
    if (!cum_lengths) abort();
    if (row_count == 0)
        cum_lengths[0] = len;
    else
        cum_lengths[row_count] = cum_lengths[row_count - 1] + len;

    row_count++;                              /* one more row stored */
}

/* Return a(n) : the nth element of the linearized triangle. */
seq_t a(idx_t n)
{
    /* Ensure enough rows are built to cover index n. */
    while (row_count == 0 || cum_lengths[row_count - 1] <= n)
        build_row((idx_t)row_count);

    /* Locate the row containing index n using binary search. */
    size_t lo = 0, hi = row_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (cum_lengths[mid] > n)
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t row = lo;                           /* row (0‑based) */

    idx_t row_start = (row == 0) ? 0 : cum_lengths[row - 1];
    idx_t k = n - row_start;                   /* column in this row */

    return rows_data[row][(size_t)k];
}
