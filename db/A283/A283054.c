/*
 * A283054  Triangle read by rows: T(n,k) = T(n,k-1) + T(n-1,k), T(n,0)=1,
 *          T(n,n) = T(n,n-1) + 1.
 * https://oeis.org/A283054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 75, u32 n <= 227, u64 n <= 736,
 *            u128 n <= 2482, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A283054: Triangle read by rows  
    Recurrence:   T(n,0)   = 1                     (n ≥ 0)  
                  T(n,k)   = T(n,k-1) + T(n-1,k)   (0 < k < n)  
                  T(n,n)   = T(n,n-1) + 1           (n ≥ 0)  
    a(n) is the entry located at index n in row‑major order, i.e.  
    with r = floor((sqrt(8n+1)-1)/2) and k = n - r*(r+1)/2, a(n)=T(r,k).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#  include <new>
#endif

static seq_t **rows = NULL;   /* rows[i] points to array of length i+1 */
static size_t   max_rows = 0; /* number of rows currently allocated */

/* Ensure that rows 0 … r are computed and stored. */
static void
ensure_rows(idx_t r)
{
    /* Grow the pointer array if necessary. */
    if (rows == NULL) {
        rows = (seq_t**)malloc((size_t)(r + 1) * sizeof(seq_t*));
        if (!rows) abort();
    } else if (max_rows < (size_t)(r + 1)) {
        seq_t **tmp = (seq_t**)realloc(rows, (size_t)(r + 1) * sizeof(seq_t*));
        if (!tmp) abort();
        rows = tmp;
    }

    /* Build new rows from max_rows up to r inclusive. */
    for (idx_t i = (idx_t)max_rows; i <= r; ++i) {
#if defined(__cplusplus)
        seq_t *row = new seq_t[(size_t)(i + 1)];
#else
        seq_t *row = (seq_t*)malloc((size_t)(i + 1) * sizeof(seq_t));
        if (!row) abort();
#endif
        row[0] = (seq_t)1;
        for (idx_t k = 1; k <= i; ++k) {
            if (k == i) {
                row[k] = row[k - 1] + (seq_t)1;
            } else {
                row[k] = row[k - 1] + rows[i - 1][k];
            }
        }
        rows[i] = row;
    }
    max_rows = (size_t)(r + 1);
}

/* Return the nth term of the sequence. */
seq_t
a(idx_t n)
{
    /* Determine the row r that contains position n. */
    idx_t r = 0;
    while ((idx_t)(r + 1) * (idx_t)(r + 2) / 2 <= n) ++r;

    /* Ensure rows up to r are built. */
    ensure_rows(r);

    /* Compute column index k, then return the value. */
    idx_t start = (idx_t)r * (idx_t)(r + 1) / 2;
    idx_t k = n - start;
    return rows[r][k];
}
