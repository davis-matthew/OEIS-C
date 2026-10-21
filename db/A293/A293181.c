/*
 * A293181  Irregular triangle read by rows: T(n,k) is the number of
 *          k-partitions of {1..2n} that are invariant under a permutation
 *          consisting of n 2-cycles (1 <= k <= 2n).
 * https://oeis.org/A293181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 163, u64 n <= 471,
 *            u128 n <= 1281, big any n
 * Helpers:   _row_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A293181: irregular triangle read by rows.  
   Triangle T(m,k) is defined by  
          T(0,0) = 1  
   and for m ≥ 1 (1 ≤ k ≤ 2m)  
          T(m,k) = k*T(m‑1,k) + T(m‑1,k‑1) + T(m‑1,k‑2)  
   (indices outside 0 … 2(m‑1) are treated as 0).  
   The sequence a(n) is the n‑th element of the triangle read in
   row‑major order, with offset 1. */

#include <stdlib.h>
#include <string.h>
#include "../utils/_row_for_idx.h"

/* Dynamically allocated storage for the triangle rows. */
static seq_t **_tri_rows = NULL;   /* pointers to rows 0,1,2,… */
static size_t  _tri_cap  = 0;      /* allocated capacity for row pointers */
static size_t  _tri_cnt  = 0;      /* number of rows currently computed */

/* Ensure the row‑pointer array has capacity for at least 'need' rows. */
static void _ensure_rows_cap(size_t need)
{
    if (_tri_cap >= need) return;
    size_t new_cap = _tri_cap ? _tri_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t **tmp = (seq_t **)malloc(new_cap * sizeof(seq_t *));
    if (!tmp) abort();
    if (_tri_rows) {
        memcpy(tmp, _tri_rows, _tri_cnt * sizeof(seq_t *));
        free(_tri_rows);
    }
    _tri_rows = tmp;
    _tri_cap  = new_cap;
}

/* Compute all rows up to and including row 'up_to' (0‑based). */
static void _ensure_rows_until(size_t up_to)
{
    for (size_t r = _tri_cnt; r <= up_to; ++r) {
        _ensure_rows_cap(r + 1);
        /* Row r has length 2*r+1 (indices 0 … 2*r). */
        size_t len = 2 * r + 1;
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) abort();

        if (r == 0) {
            row[0] = (seq_t)1;               /* T(0,0) = 1 */
        } else {
            row[0] = (seq_t)0;               /* unused but harmless */
            size_t prev_len = 2 * (r - 1) + 1; /* indices 0 … 2*(r‑1) */
            for (size_t k = 1; k <= 2 * r; ++k) {
                seq_t val = (seq_t)0;
                if (k < prev_len)                 val += (seq_t)k * _tri_rows[r - 1][k];
                if (k - 1 < prev_len)             val += _tri_rows[r - 1][k - 1];
                if (k >= 2 && k - 2 < prev_len)   val += _tri_rows[r - 1][k - 2];
                row[k] = val;
            }
        }

        _tri_rows[r] = row;
        ++_tri_cnt;
    }
}

/* ---------------------------------------------------------- */
/* Public function required by the specification.            */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* safety for out‑of‑range calls */

    size_t r   = _row_for_idx(n);                   /* 1‑based row number */
    size_t pos = (size_t)n - (r - 1) * r;           /* 1‑based column index */

    if (_tri_cnt <= r) _ensure_rows_until(r);       /* build up to row r */

    return _tri_rows[r][pos];
}
