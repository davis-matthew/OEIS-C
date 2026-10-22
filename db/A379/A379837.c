/*
 * A379837  Triangle read by rows formed using Pascal's rule except that n-th
 *          row begins and ends with Fibonacci(n+3).
 * https://oeis.org/A379837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _row_start, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A379837:  Triangle read by rows using Pascal's rule, with each
 * n‑th row beginning and ending with Fibonacci(n+3).  a(n) is the
 * n‑th element of the flattened triangle. */

#include <stdlib.h>
#include "../utils/fib.h"
#include "../utils/_row_start.h"

/* dynamic array of pointers to rows; rows[i] points to the row of length
 * i+1 (0‑based). */
static seq_t **rows = NULL;
static size_t rows_cnt = 0;   /* number of rows currently stored */
static size_t rows_cap = 0;   /* capacity of the rows array */

/* find the smallest row r such that cumulative length up to r exceeds n
 * (i.e. cum(r) = (r+1)(r+2)/2 > n).  r is 0‑based. */
static idx_t _row_of_index(idx_t n)
{
    idx_t low = 0, high = 1;
    /* grow high until cum(high) > n */
    while (((unsigned __int128)(high + 1) * (high + 2)) / 2 <= n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)(mid + 1) * (mid + 2)) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;   /* smallest r with cum(r) > n */
}

/* grow the rows cache up to and including row 'req' */
static void _ensure_rows(idx_t req)
{
    while (rows_cnt <= (size_t)req) {
        size_t len = rows_cnt + 1;                 /* length of new row */
        /* allocate raw memory for the row; cast for C++ compatibility */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) exit(1);                         /* allocation fail */

        seq_t end_val = fib((idx_t)rows_cnt + 3);
        row[0] = end_val;
        row[len - 1] = end_val;

        if (rows_cnt > 0) {                        /* fill interior */
            seq_t *prev = rows[rows_cnt - 1];
            for (size_t i = 1; i < len - 1; ++i)
                row[i] = prev[i - 1] + prev[i];
        }

        if (rows_cnt >= rows_cap) {                /* grow the array of pointers */
            size_t new_cap = rows_cap ? rows_cap * 2 : 4;
            /* realloc returns void*, cast for C++ */
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp) exit(1);
            rows = tmp;
            rows_cap = new_cap;
        }

        rows[rows_cnt++] = row;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r = _row_of_index(n);          /* row containing index n */
    idx_t start = _row_start(r);         /* first index of row r */
    idx_t col   = n - start;             /* column within the row */
    _ensure_rows(r);                     /* compute rows up to r if needed */
    return rows[r][(size_t)col];
}
