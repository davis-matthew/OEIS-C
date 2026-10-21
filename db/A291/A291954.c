/*
 * A291954  Triangle read by rows: T(n,k) = T(n-k,k-1) - T(n-k,k) with T(0,0)
 *          = 1 for 0 <= k <= A003056(n).
 * https://oeis.org/A291954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 965, i16 n <= 4672, i32 n <= 9981, i64 n <= 9981,
 *            i128 n <= 9981, big any n
 * Helpers:   kmax_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A291954 – Triangle read by rows
 *
 *  Definition:  T(0,0) = 1.
 *                T(n,k) = T(n-k,k-1) – T(n-k,k)   for 0 < k ≤ kmax(n)
 *  where kmax(n) = ⌊(√(8n+1)-1)/2⌋.
 *
 *  The sequence a(n) is obtained by writing the triangle
 *  row by row (row 0 first, then row 1, …) and reading off the
 *  entries linearly.  Example:  a(0)=1, a(1)=0, a(2)=1,
 *  a(3)=0, a(4)=-1, …
 *
 *  This implementation builds the triangle lazily and caches all
 *  rows that are required.  The cached data survive between
 *  calls to a() and are recomputed only when a larger index is
 *  requested.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/kmax_of.h"

/* Return the n‑th term of sequence A291954.  */
seq_t a(idx_t n)
{
    /* ---------- static cache variables ---------- */
    static idx_t *row_start = NULL;   /* start index of each row in flat data array */
    static size_t row_cap   = 0;      /* allocated capacity of row_start array    */
    static size_t row_count = 0;      /* number of rows already computed         */

    static seq_t *data      = NULL;   /* flattened triangle values                */
    static size_t data_cap  = 0;      /* allocated capacity of data array         */
    static size_t data_count= 0;      /* number of values stored so far           */

    /* ---------- initialise caches on first call ---------- */
    if (!row_start) {
        row_cap   = 16;
        row_start = (idx_t *)malloc(row_cap * sizeof(idx_t));
        if (!row_start) return (seq_t)0;
        row_start[0] = 0;            /* first row starts at linear index 0 */

        data_cap   = 16;
        data       = (seq_t *)malloc(data_cap * sizeof(seq_t));
        if (!data) return (seq_t)0;
    }

    /* ---------- build rows until we cover index n ---------- */
    while (data_count <= n) {
        /* Ensure row_start has space for the new row index+1 */
        if (row_count + 1 >= row_cap) {
            row_cap <<= 1;
            idx_t *new_start = (idx_t *)realloc(row_start, row_cap * sizeof(idx_t));
            if (!new_start) return (seq_t)0;
            row_start = new_start;
        }

        idx_t cur_row = (idx_t)row_count;          /* current row number being built */
        idx_t len = kmax_of(cur_row) + 1;          /* number of entries in this row  */

        /* Ensure data array has enough space for new values */
        if (data_count + (size_t)len > data_cap) {
            data_cap = (data_count + (size_t)len) * 2;
            seq_t *new_data = (seq_t *)realloc(data, data_cap * sizeof(seq_t));
            if (!new_data) return (seq_t)0;
            data = new_data;
        }

        /* Compute each entry of the current row */
        for (idx_t k = 0; k < len; ++k) {
            seq_t val;
            if (cur_row == 0) {                     /* row 0: only T(0,0)=1 */
                val = (k == 0) ? (seq_t)1 : (seq_t)0;
            } else if (k == 0) {                    /* all other rows start with 0 */
                val = (seq_t)0;
            } else {                                /* recursion */
                idx_t left_row = cur_row - k;
                idx_t kmax_left = kmax_of(left_row);

                seq_t left  = (k - 1 <= kmax_left) ? data[row_start[left_row] + (k - 1)] : (seq_t)0;
                seq_t right = (k <= kmax_left)     ? data[row_start[left_row] + k]     : (seq_t)0;
                val = left - right;
            }
            data[data_count + k] = val;
        }

        data_count += len;
        row_start[cur_row + 1] = data_count;    /* start index of next row */
        ++row_count;
    }

    /* data[n] now holds the requested term */
    return data[n];
}
