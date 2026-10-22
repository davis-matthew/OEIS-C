/*
 * A383609  Triangle read by rows: T(n,k) = T(n-1, k-2) + T(n-1, k-1) + T(n-1,
 *          k) for 0 < k < n, T(n,0) = T(n,n) = 1.
 * https://oeis.org/A383609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = T(n-1,k-2) + T(n-1,k-1) + T(n-1,k),
   with T(n,0) = T(n,n) = 1.
   a(n) is the element at index n of the triangle read by rows. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_triangular.h"

static seq_t **_t_rows = NULL;          /* array of pointers to each row */
static size_t _t_rows_allocated = 0;    /* number of rows allocated */

/* Ensure that _t_rows has at least idx+1 slots, all initialized to NULL */
static void _ensure_rows_allocated(size_t idx) {
    if (_t_rows_allocated > idx)
        return;
    size_t new_alloc = idx + 1;
    /* realloc returns void* in both C and C++; cast to the proper type */
    seq_t **tmp = (seq_t**)realloc(_t_rows, new_alloc * sizeof(seq_t*));
    if (!tmp && new_alloc > 0) abort();
    _t_rows = tmp;
    for (size_t i = _t_rows_allocated; i < new_alloc; ++i)
        _t_rows[i] = NULL;
    _t_rows_allocated = new_alloc;
}

/* Compute the (r+1)-th row of the triangle and store it in _t_rows[r]. */
static void _compute_row(size_t r) {
    _ensure_rows_allocated(r);
    if (_t_rows[r] != NULL)   /* already computed */
        return;
    if (r > 0)
        _compute_row(r - 1);   /* ensure the previous row is ready */

    size_t len = r + 1;
    /* allocate memory for the row; cast from void* for C++ */
    seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
    if (!row) abort();

    row[0] = (seq_t)1;
    row[r] = (seq_t)1;
    for (size_t k = 1; k < r; ++k) {
        seq_t val = 0;
        if (k >= 2)                /* T(r-1,k-2) */
            val += _t_rows[r - 1][k - 2];
        val += _t_rows[r - 1][k - 1];/* T(r-1,k-1) */
        val += _t_rows[r - 1][k];    /* T(r-1,k)   */
        row[k] = val;
    }
    _t_rows[r] = row;
}

static idx_t _row_from_index(idx_t n) {
    idx_t low = 0, high = 1;
    /* double high until triangular(high) > n */
    while (_triangular(high) <= n) {
        high <<= 1;
        if (high == 0) break;        /* overflow guard, though unlikely here */
    }
    /* binary search between low and high */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (_triangular(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main entry function required by the judge */
seq_t a(idx_t n) {
    idx_t r = _row_from_index(n);
    idx_t before = _triangular(r);
    idx_t c = n - before;          /* column within the row */

    _compute_row((size_t)r);
    return _t_rows[(size_t)r][(size_t)c];
}
