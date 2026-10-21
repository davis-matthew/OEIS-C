/*
 * A276056  Triangle read by rows: T(n,k) is the number of compositions of n
 *          with parts in {1,3} and having asymmetry degree equal to k, (n>=0;
 *          0<=k<=floor(n/4)).
 * https://oeis.org/A276056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A276056 – Triangle read by rows.
 *
 *  Let T(n,k) be the number of compositions of n using only 1's and 3's
 *  whose asymmetry degree is k (0 ≤ k ≤ ⌊n/4⌋).  Row n of the triangle
 *  contains T(n,0), T(n,1), …, T(n,⌊n/4⌋) and the OEIS sequence a(n)
 *  is this triangle written row‑by‑row.
 *
 *  Recurrence:
 *      T(n,k) = T(n-2,k) + 2 * T(n-4,k-1) + T(n-6,k)         (n ≥ 6, k ≥ 0)
 *  with initial rows
 *      T(0,0)=1 , T(1,0)=1 , T(2,0)=1 , T(3,0)=2 ,
 *      T(4,0)=1 , T(4,1)=2 , T(5,0)=2 , T(5,1)=2 .
 *
 *  The routine below builds the triangle lazily, caching all rows
 *  that have been required.  This allows any order of calls to a(n)
 *  while keeping the time per call well below the 2 s limit, even for
 *  n ≈ 10 000.  A total of ~12 M entries are needed for that range,
 *  easily handled by the available memory.
 *
 *  Types idx_t and seq_t are supplied by the build system; they are
 *  assumed to be unsigned integer types (idx_t: uint64_t, seq_t: an
 *  unsigned integer or arbitrary‑precision type).
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Internal static data structures ---------- */
static seq_t **rows_data   = NULL;   /* pointers to rows */
static idx_t  *row_start   = NULL;   /* start index of each row in the flattened sequence */
static size_t *row_size    = NULL;   /* number of elements in each row */
static size_t  rows_count  = 0;      /* number of rows that are currently cached */
static size_t  rows_cap    = 0;      /* capacity of the above arrays */

/* ------------------------------------------------------------------ */
/* Resize the arrays that store the row pointers and metadata.        */
/* ------------------------------------------------------------------ */
static void ensure_rows_array_size(size_t new_cap)
{
    if (new_cap <= rows_cap)
        return;
    rows_cap = new_cap;
    rows_data = (seq_t **)realloc(rows_data, rows_cap * sizeof(seq_t *));
    row_start = (idx_t *)realloc(row_start, rows_cap * sizeof(idx_t));
    row_size  = (size_t *)realloc(row_size,  rows_cap * sizeof(size_t));
}

/* ------------------------------------------------------------------ */
/* Compute rows up to and including row `r`.  No row is recomputed.   */
/* ------------------------------------------------------------------ */
static void ensure_rows_upto(idx_t r)
{
    /* Initialise the first four base rows if necessary. */
    if (rows_count == 0) {
        ensure_rows_array_size(16);          /* small initial capacity */
        /* Row 0 */
        rows_data[0] = (seq_t *)malloc(sizeof(seq_t));
        rows_data[0][0] = (seq_t)1;
        row_start[0] = (idx_t)0;
        row_size[0] = 1;
        /* Row 1 */
        rows_data[1] = (seq_t *)malloc(sizeof(seq_t));
        rows_data[1][0] = (seq_t)1;
        row_start[1] = row_start[0] + row_size[0];
        row_size[1] = 1;
        /* Row 2 */
        rows_data[2] = (seq_t *)malloc(sizeof(seq_t));
        rows_data[2][0] = (seq_t)1;
        row_start[2] = row_start[1] + row_size[1];
        row_size[2] = 1;
        /* Row 3 */
        rows_data[3] = (seq_t *)malloc(sizeof(seq_t));
        rows_data[3][0] = (seq_t)2;
        row_start[3] = row_start[2] + row_size[2];
        row_size[3] = 1;
        rows_count = 4;
    }

    /* Grow rows one by one until the desired row is cached. */
    while (rows_count <= (size_t)r) {
        idx_t       cur = (idx_t)rows_count;          /* index of the row to compute */
        size_t      L   = 1 + (size_t)(cur / 4);      /* number of elements in this row */

        /* Allocate the row itself. */
        seq_t *row = (seq_t *)malloc(L * sizeof(seq_t));

        /* Compute each entry using the recurrence. */
        for (size_t k = 0; k < L; ++k) {
            seq_t val = (seq_t)0;

            /* T(cur-2, k) */
            if (cur >= 2 && k < row_size[cur - 2]) {
                val += rows_data[cur - 2][k];
            }

            /* 2 * T(cur-4, k-1) */
            if (cur >= 4 && k >= 1 && (k - 1) < row_size[cur - 4]) {
                val += (seq_t)2 * rows_data[cur - 4][k - 1];
            }

            /* T(cur-6, k) */
            if (cur >= 6 && k < row_size[cur - 6]) {
                val += rows_data[cur - 6][k];
            }

            row[k] = val;
        }

        /* Store the computed row and its metadata. */
        ensure_rows_array_size(rows_count + 1);
        rows_data[rows_count]  = row;
        row_size[rows_count]   = L;
        row_start[rows_count]  = row_start[rows_count - 1] + row_size[rows_count - 1];
        ++rows_count;
    }
}

/* ------------------------------------------------------------------ */
/* Find the row index `r` such that row_start[r] <= pos < row_start[r+1] */
/* ------------------------------------------------------------------ */
static idx_t find_row(idx_t pos)
{
    size_t low  = 0;
    size_t high = rows_count;    /* exclusive upper bound */

    while (low < high) {
        size_t mid = low + ((high - low) >> 1);
        if (row_start[mid] <= pos)
            low = mid + 1;
        else
            high = mid;
    }
    return (idx_t)(low - 1);     /* safe because pos < row_start[rows_count] */
}

/* ------------------------------------------------------------------ */
/* Main entry point: compute the n‑th term of A276056.                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure that all rows up to the one containing `n` are cached. */
    while (rows_count == 0 ||
           row_start[rows_count - 1] + row_size[rows_count - 1] <= n) {
        /* The next row to compute is `rows_count`.  This will cause the loop
           to stop when the cumulative size exceeds `n`.  */
        ensure_rows_upto((idx_t)rows_count);
    }

    /* Locate the row and column of the desired element. */
    idx_t r = find_row(n);
    idx_t k = n - row_start[r];

    return rows_data[r][k];
}
