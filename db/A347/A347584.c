/*
 * A347584  Triangle formed by Pascal's rule, except that the n-th row begins
 *          and ends with the n-th Lucas number.
 * https://oeis.org/A347584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The terms a(n) are the row-major enumeration of a triangle that
 *            obeys the Pascal rule with the n-th Lucas number at each row
 *            end. The recurrence is: T[0][0] = 2 T[1][0] = T[1][1] = 1 For
 *            r>=2, T[r][0]=T[r][r] = Lucas(r) For 0<k<r: T[r][k] =
 *            T[r-1][k-1] + T[r-1][k] The sequence a(n) is defined by...
 * Range:     u8 n <= 48, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/_grow_seq.h"

/* Forward declaration of the sequence function. */
seq_t a(idx_t n);

/* ---------- Internal static data and helpers ---------- */

static size_t _row_count = 0;          /* Number of rows already computed */
static seq_t * _vals = NULL;           /* Flat storage of all entries */
static size_t _vals_len = 0;           /* Current number of stored entries */
static size_t _vals_cap = 0;           /* Capacity of _vals array          */
static seq_t _lucas_prev1 = 0;         /* Lucas(n-1) */
static seq_t _lucas_prev2 = 0;         /* Lucas(n-2) */

/* Compute Lucas(n) and store the next row of the triangle. */
static void _ensure_row(idx_t target_row)
{
    /* target_row is the highest row that must exist */
    if (_row_count > target_row)
        return;

    for (idx_t r = _row_count; r <= target_row; ++r) {
        seq_t lucas;
        if (r == 0)
            lucas = 2;                 /* L0 */
        else if (r == 1)
            lucas = 1;                 /* L1 */
        else
            lucas = _lucas_prev1 + _lucas_prev2;

        size_t len = (size_t)(r + 1);

        /* Allocate space for the new row */
        _grow_seq(&_vals, &_vals_cap, _vals_len + len);
        size_t base = _vals_len;

        if (r == 0) {
            _vals[base] = lucas;
        } else {
            /* First element */
            _vals[base] = lucas;

            /* Interior elements */
            size_t prev_start = (size_t)((r - 1) * r / 2);
            for (size_t k = 1; k < (size_t)r; ++k) {
                _vals[base + k] = _vals[prev_start + (k - 1)] +
                                  _vals[prev_start + k];
            }
            /* Last element */
            _vals[base + r] = lucas;
        }

        _vals_len += len;

        /* Update Lucas previous values for next iteration */
        _lucas_prev2 = _lucas_prev1;
        _lucas_prev1 = lucas;
    }
    _row_count = target_row + 1;
}

/* --------------- Public interface --------------------- */

seq_t a(idx_t n)
{
    /* Determine the row 'r' that contains the index n.
       Row r starts at position start = r*(r+1)/2 and has length r+1.
       Find r such that start <= n < start + r + 1. */
    idx_t r = 0;
    /* Simple linear search is fine because r is about sqrt(2n). */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Ensure the required row has been computed. */
    _ensure_row(r);

    idx_t start = r * (r + 1) / 2;      /* starting index of row r */
    idx_t k = n - start;                /* offset within the row */

    return _vals[start + k];
}
