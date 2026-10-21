/*
 * A275438  Triangle read by rows: T(n,k) is the number of compositions of n
 *          with parts in {1,2} having asymmetry degree equal to k (n>=0;
 *          0<=k<=floor(n/3)).
 * https://oeis.org/A275438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A275438 – T(n,k) recurrence
 *   T(n,k) = T(n-2,k) + 2*T(n-3,k-1) + T(n-4,k)   for n>=4,
 *   with base rows:
 *      T(0,0) = 1,   T(1,0) = 1,   T(2,0) = 2.
 *   Row n has 1+floor(n/3) entries (k ranges 0..floor(n/3)).
 *   The flattened sequence a(n) is obtained by reading the triangle
 *   row by row.  This function memoises the triangle and returns a(n)
 *   for any n≥0 (calls may come in any order).
 */

#include <stdlib.h>
#include <stdint.h>

/* Static tables storing the rows of the triangle */
static seq_t **rows = NULL;          /* array of pointers to rows */
static size_t   rows_count = 0;      /* number of rows currently stored */
static size_t   rows_cap   = 0;      /* allocated capacity of 'rows' */

/* Prefix sums of row lengths (total number of values up to each row) */
static uint64_t *prefix = NULL;      /* cumulative counts of rows */
static size_t    prefix_count = 0;   /* number of rows recorded so far */
static size_t    prefix_cap   = 0;   /* allocated capacity of 'prefix' */

/* Ensure the 'rows' array has room for at least 'new_cap' entries. */
static void grow_rows_array(void)
{
    size_t new_cap = rows_cap == 0 ? 64 : rows_cap << 1;
    seq_t **new_rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
    if (!new_rows) abort();
    rows = new_rows;
    rows_cap = new_cap;
}

/* Ensure the 'prefix' array has room for at least 'new_cap' entries. */
static void grow_prefix_array(void)
{
    size_t new_cap = prefix_cap == 0 ? 64 : prefix_cap << 1;
    uint64_t *new_prefix = (uint64_t*)realloc(prefix, new_cap * sizeof(uint64_t));
    if (!new_prefix) abort();
    prefix = new_prefix;
    prefix_cap = new_cap;
}

/* Create rows up to and including 'up_to' (row index), updating 'prefix'. */
static void ensure_rows_and_prefix(size_t up_to)
{
    for (; rows_count <= up_to; ++rows_count) {
        size_t n   = rows_count;
        size_t len = 1 + n / 3;               /* number of k‑values in this row */

        seq_t *row_arr = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row_arr) abort();

        /* Base rows */
        if (n == 0) {
            row_arr[0] = (seq_t)1;
        } else if (n == 1) {
            row_arr[0] = (seq_t)1;
        } else if (n == 2) {
            row_arr[0] = (seq_t)2;
        } else {
            /* Row lengths of predecessor rows (0 if the row does not exist) */
            size_t len_n2 = 0, len_n3 = 0, len_n4 = 0;
            if (n >= 2) len_n2 = 1 + (n - 2) / 3;
            if (n >= 3) len_n3 = 1 + (n - 3) / 3;
            if (n >= 4) len_n4 = 1 + (n - 4) / 3;

            for (size_t k = 0; k < len; ++k) {
                seq_t val = 0;

                /* T(n-2,k) term */
                if (n >= 2 && k < len_n2)
                    val += rows[n - 2][k];

                /* 2*T(n-3,k-1) term */
                if (n >= 3 && k > 0 && (k - 1) < len_n3)
                    val += rows[n - 3][k - 1] * (seq_t)2;

                /* T(n-4,k) term */
                if (n >= 4 && k < len_n4)
                    val += rows[n - 4][k];

                row_arr[k] = val;
            }
        }

        /* Store the new row. */
        if (rows_count == rows_cap) grow_rows_array();
        rows[rows_count] = row_arr;

        /* Update prefix sums. */
        uint64_t added = (uint64_t)len;
        if (prefix_count == prefix_cap) grow_prefix_array();
        if (prefix_count == 0) {
            prefix[0] = added;
        } else {
            prefix[prefix_count] = prefix[prefix_count - 1] + added;
        }
        ++prefix_count;
    }
}

/* Binary search on 'prefix' to find the first row whose cumulative count exceeds 'pos'. */
static size_t find_row_for_pos(uint64_t pos)
{
    size_t lo = 0;
    size_t hi = prefix_count;            /* exclusive upper bound */
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (prefix[mid] <= pos)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/*
 *  Return the a(n) value for the flattened triangle.
 *
 *  Parameters:
 *     n : the 0‑based index in the flattened triangle
 *
 *  Returns:
 *     seq_t value of a(n)
 */
seq_t a(idx_t n)
{
    uint64_t pos = (uint64_t)n;   /* n fits in 64 bits by definition */

    /* Make sure at least row 0 is computed. */
    if (prefix_count == 0)
        ensure_rows_and_prefix(0);

    /* Determine row containing the desired entry. */
    size_t row = find_row_for_pos(pos);

    /* Ensure rows up to 'row' are available. */
    if (row >= rows_count)
        ensure_rows_and_prefix(row);

    uint64_t prev_sum = (row == 0) ? 0 : prefix[row - 1];
    size_t   k       = (size_t)(pos - prev_sum);

    return rows[row][k];
}
