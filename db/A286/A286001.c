/*
 * A286001  A table of partitions into consecutive parts (see Comments lines
 *          for definition).
 * https://oeis.org/A286001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the element in the triangle read by rows, where column k
 *            lists successive blocks of k consecutive integers. The m-th
 *            block starts with m, and the first element of column k is at row
 *            r = k*(k+1)/2. The sequence a(n) is obtained by traversing the
 *            triangle row‑by‑row
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   compute_row_max_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include "../utils/compute_row_max_k.h"

seq_t a(idx_t n)
{
    /* Triangular prefix sums: cum[i] = total entries up to row i (1‑based) */
    static idx_t *cum = NULL;
    static size_t cum_cap = 0;
    static idx_t cum_rows = 0;   /* number of rows that have been filled, cum[0] = 0 */

    if (cum == NULL) {
        cum_cap = 16;
        cum = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        cum[0] = 0;
        cum_rows = 0;
    }

    /* Extend cum until it contains enough rows to cover index n */
    while (cum[cum_rows] < n) {
        idx_t next_row = cum_rows + 1;                        /* 1‑based row number */
        idx_t k = compute_row_max_k(next_row);                /* number of columns in this row */

        if (cum_rows + 1 >= cum_cap) {
            cum_cap *= 2;
            cum = (idx_t *)realloc(cum, cum_cap * sizeof(idx_t));
        }
        ++cum_rows;
        cum[cum_rows] = cum[cum_rows - 1] + k;
    }

    /* Binary search for row r such that cum[r-1] < n <= cum[r] */
    idx_t lo = 1, hi = cum_rows;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;

    /* Column index within this row (1‑based) */
    idx_t col = n - cum[row - 1];

    /* Compute the value at (row, col) in the triangle */
    idx_t start_row = col * (col + 1) / 2;     /* first row where column col appears */
    idx_t shift = row - start_row;
    idx_t block = shift / col + 1;             /* block number (starting at 1) */
    idx_t offset = shift % col;                /* position within block */

    seq_t value = (seq_t)block + (seq_t)offset;
    return value;
}
