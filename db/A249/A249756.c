/*
 * A249756  Triangular array of coefficients of polynomials p(n,x) = (x +
 *          1)*p(n-1,x) + n*x, p(0,x) = 1.
 * https://oeis.org/A249756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = coefficient of x^k in p(r,x), where p(0,x)=1 and
 *            p(n,x)=(x+1)*p(n-1,x)+n*x (OEIS A249756)
 * Range:     u8 n <= 47, u16 n <= 160, u32 n <= 608, u64 n <= 2241,
 *            u128 n <= 5252, big any n
 * Helpers:   _row_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In C this program uses malloc(); in C++ cast explicitly
   to the appropriate pointer type. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_row_index.h"

/* The types seq_t and idx_t are supplied by the build system. */
extern seq_t a(idx_t n);  /* prototype for the compiler */

static seq_t **_rows = NULL;   /* cached rows of coefficients */
static idx_t  _nrow = 0;       /* number of rows currently stored (max index+1) */

/* ------------------------------------------------------------------ */
/* Ensure that rows 0 … r are computed and cached.                   */
/* ------------------------------------------------------------------ */
static void _ensure_rows(idx_t r)
{
    if (_nrow > r) return;                     /* already available */

    /* Grow the array holding row pointers. */
    _rows = (seq_t **)(realloc(_rows, (r + 1) * sizeof(seq_t *)));
    if (_rows == NULL) abort();

    while (_nrow <= r)
    {
        if (_nrow == 0)                         /* base case: p(0,x) = 1 */
        {
            _rows[0] = (seq_t *)(malloc(sizeof(seq_t)));
            if (_rows[0] == NULL) abort();
            _rows[0][0] = (seq_t)1;
            _nrow = 1;
            continue;
        }

        seq_t *prev        = _rows[_nrow - 1];
        idx_t prev_len     = _nrow;              /* length of previous row */
        idx_t cur_len      = prev_len + 1;       /* length of current row */

        seq_t *cur = (seq_t *)(malloc(cur_len * sizeof(seq_t)));
        if (cur == NULL) abort();

        /* Build the new row from the previous one. */
        cur[0] = prev[0];                        /* from the "*1" factor */
        for (idx_t k = 1; k < prev_len; ++k)
            cur[k] = prev[k] + prev[k - 1];       /* "*1 + *x" contribution */
        cur[prev_len] = prev[prev_len - 1];      /* *x only */

        /* Add the n*x term: coefficient of x^1 is increased by current row index. */
        cur[1] += (seq_t)(_nrow);

        /* Store and advance. */
        _rows[_nrow] = cur;
        ++_nrow;
    }
}

/* ------------------------------------------------------------------ */
/* Find the row index r such that T(r)=r(r+1)/2 ≤ n < T(r+1).         */

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t r = _row_index(n);                                 /* row index   */
    unsigned __int128 tr = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tr;                                 /* column in row */

    _ensure_rows(r);                                         /* compute rows as needed */

    return _rows[r][k];                                      /* coefficient */
}
