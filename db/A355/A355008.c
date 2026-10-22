/*
 * A355008  Triangle T(n, k), n > 0, k = 1..n, read by rows; each row contains
 *          the smallest distinct positive integers (in ascending order) such
 *          that a term of row n times a term of row n+1 does not appear in
 *          row n+2.
 * https://oeis.org/A355008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8126, u16 n <= 9484, u32 n <= 9484, u64 n <= 9484,
 *            u128 n <= 9484, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle T(n,k) – on each row choose the smallest positive integers
    so that for all a in row n, b in row n+1 the product a*b
    does not appear in row n+2.
    The function a(n) returns the n‑th term of the flat list of rows. */

#include <stdlib.h>
#include <stdbool.h>

/* Allocate or grow an array of seq_t* (row pointers) */
static seq_t **_row_ptrs = NULL;  /* indexed 1.._max_row; _row_ptrs[0] unused */
static size_t _max_row = 0;

/* Binary search for a value in a sorted array of seq_t. */
static bool contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == val) return true;
        if (arr[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* Ensure that all rows up to 'need' have been constructed. */
static void _ensure_rows(size_t need)
{
    if (need <= _max_row) return;

    /* enlarge the pointer array */
    seq_t **tmp = (seq_t**)realloc(_row_ptrs, (need + 1) * sizeof(seq_t*));
    if (!tmp) abort();
    _row_ptrs = tmp;
    for (size_t i = _max_row + 1; i <= need; ++i) _row_ptrs[i] = NULL;

    /* base rows */
    if (_row_ptrs[1] == NULL) {
        _row_ptrs[1] = (seq_t*)malloc(sizeof(seq_t));
        _row_ptrs[1][0] = 1;
    }
    if (_row_ptrs[2] == NULL) {
        _row_ptrs[2] = (seq_t*)malloc(2 * sizeof(seq_t));
        _row_ptrs[2][0] = 1;
        _row_ptrs[2][1] = 2;
    }
    if (_row_ptrs[3] == NULL) {
        _row_ptrs[3] = (seq_t*)malloc(3 * sizeof(seq_t));
        _row_ptrs[3][0] = 3;
        _row_ptrs[3][1] = 4;
        _row_ptrs[3][2] = 5;
    }

    /* construct remaining rows */
    size_t start = (_max_row < 4) ? 4 : _max_row + 1;
    for (size_t r = start; r <= need; ++r) {
        seq_t *row = (seq_t*)malloc(r * sizeof(seq_t));
        const seq_t *prev2 = _row_ptrs[r - 2];
        const seq_t *prev1 = _row_ptrs[r - 1];
        size_t taken = 0;
        seq_t cand = 1;
        while (taken < r) {
            bool banned = false;
            /* examine all divisor pairs of cand */
            for (idx_t d = 1; (seq_t)d * (seq_t)d <= cand; ++d) {
                if (cand % (seq_t)d != 0) continue;
                seq_t a = d, b = cand / d;
                if (contains(prev2, r - 2, a) && contains(prev1, r - 1, b)) {
                    banned = true; break;
                }
                if (d != b) {
                    a = b; b = d;
                    if (contains(prev2, r - 2, a) && contains(prev1, r - 1, b)) {
                        banned = true; break;
                    }
                }
            }
            if (!banned) {
                row[taken++] = cand;
            }
            ++cand;
        }
        _row_ptrs[r] = row;
    }
    _max_row = need;
}

/* public API */
seq_t a(idx_t n)
{
    /* find the row containing position n (rows are triangularly indexed) */
    size_t row = 1;
    while (((seq_t)row * (seq_t)(row + 1)) / 2 < n) ++row;
    /* position within that row, zero‑based */
    size_t prev_tri = (row - 1) * (row) / 2;
    size_t offset   = (size_t)(n - prev_tri - 1);

    _ensure_rows(row);
    return _row_ptrs[row][offset];
}
