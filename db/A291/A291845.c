/*
 * A291845  Triangle, read by rows, where the g.f. of row n equals
 *          Product_{k=0..n-1} (1 + (2*k+1)*x + x^2) for n>0 with a single '1'
 *          in row 0.
 * https://oeis.org/A291845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 128, u64 n <= 303,
 *            u128 n <= 865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Triangle defined by iteratively convolving with 
   (1 + (2k+1)x + x^2) for row k≥1; row 0 = [1].
   a(n) is the n-th element of the flattened triangle (offset 0). */

static idx_t floor_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid != 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Cached rows of the triangle. */
static seq_t **rows_ptrs = NULL;   /* array of pointers to rows */
static idx_t last_row_computed = 0;/* largest row index that has been built */

/* Ensure that rows up to r (inclusive) are built. */
static void ensure_rows(idx_t r)
{
    /* Initial allocation on first call. */
    if (rows_ptrs == NULL) {
        rows_ptrs = (seq_t **)malloc(sizeof(seq_t *));
        seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
        row0[0] = (seq_t)1;
        rows_ptrs[0] = row0;
        last_row_computed = 0;
    }

    while (last_row_computed < r) {
        idx_t next   = last_row_computed + 1;
        idx_t prev_len = 2 * last_row_computed + 1;
        idx_t curr_len = 2 * next + 1;

        seq_t *curr = (seq_t *)malloc(curr_len * sizeof(seq_t));
        for (idx_t i = 0; i < curr_len; ++i)
            curr[i] = (seq_t)0;

        seq_t *prev   = rows_ptrs[last_row_computed];
        seq_t factor  = (seq_t)(2 * last_row_computed + 1); /* odd coefficient of the new factor */

        /* Convolution: prev * (1 + factor*x + x^2) */
        for (idx_t k = 0; k < prev_len; ++k) {
            curr[k]   += prev[k];
            curr[k+1] += prev[k] * factor;
            curr[k+2] += prev[k];
        }

        /* Expand the array of row pointers. */
        seq_t **tmp = (seq_t **)realloc(rows_ptrs,
                                        (last_row_computed + 2) * sizeof(seq_t *));
        rows_ptrs = tmp;
        rows_ptrs[last_row_computed + 1] = curr;

        last_row_computed = next;
    }
}

seq_t a(idx_t n)
{
    idx_t r = floor_sqrt(n);
    ensure_rows(r);

    idx_t pos = n - r * r;               /* position within row r */
    return rows_ptrs[(size_t)r][(size_t)pos];
}
