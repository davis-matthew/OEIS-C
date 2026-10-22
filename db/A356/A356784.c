/*
 * A356784  Inventory of positions as an irregular table; row 0 contains 0,
 *          subsequent rows contain the 0-based positions of 0's, followed by
 *          the position of 1's, of 2's, etc. in prior rows flattened.
 * https://oeis.org/A356784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n-th element of the inventory table described in OEIS
 *            A356784. The function builds the rows incrementally with
 *            memoisation. It is compatible with both C11 and C++17
 * Range:     u8 n <= 520, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

extern seq_t a(idx_t n);   /* forward declaration for the build system */

/* Internal memoised state.  The flattened array contains all rows generated
   so far, in row‑major order.  flat_len is the number of elements stored.
   The array is grown geometrically. */
static seq_t *flatten = NULL;
static size_t flat_len = 0;
static size_t flat_cap = 0;

/* Create a new row from the current flattened array and append it.  Rows
   are generated until the array contains at least n+1 elements. */
static void _ensure_rows_up_to(idx_t n)
{
    /* Initialise the first row if we have no data yet. */
    if (flatten == NULL) {
        flat_cap = 1;
        flatten = (seq_t *)malloc(flat_cap * sizeof(seq_t));
        if (flatten == NULL) abort();
        flatten[0] = 0;   /* row0 = [0] */
        flat_len = 1;
    }

    /* Keep generating new rows until we have the required number of terms. */
    while (flat_len <= (size_t)n) {
        size_t curr_len = flat_len;        /* length of already flattened data */

        /* Count how many times each value occurs in the current flattened data. */
        size_t *cnt   = (size_t *)calloc(curr_len, sizeof(size_t));
        size_t *offset = (size_t *)calloc(curr_len, sizeof(size_t));
        if (!cnt || !offset) {
            free(cnt); free(offset); abort();
        }

        for (size_t i = 0; i < curr_len; ++i) {
            size_t val = (size_t)flatten[i];
            if (val < curr_len) { /* values are guaranteed to be < curr_len */
                cnt[val] += 1;
            }
        }

        /* Compute the starting index for each value block in the next row. */
        offset[0] = 0;
        for (size_t v = 1; v < curr_len; ++v) {
            offset[v] = offset[v - 1] + cnt[v - 1];
        }

        /* Allocate space for the new row */
        seq_t *row = (seq_t *)malloc(curr_len * sizeof(seq_t));
        if (!row) {
            free(cnt); free(offset); abort();
        }

        /* For each position in the flattened data, place that position
           into the row at the appropriate spot for its value. */
        for (size_t i = 0; i < curr_len; ++i) {
            size_t val = (size_t)flatten[i];
            size_t pos = offset[val];
            /* store the 0‑based position (i) as a seq_t value */
            row[pos] = (seq_t)i;
            offset[val] = pos + 1;
        }

        /* Ensure the flattened array has enough capacity and append the new row. */
        if (flat_cap < flat_len + curr_len) {
            size_t new_cap = flat_cap ? flat_cap * 2 : 1;
            while (new_cap < flat_len + curr_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(flatten, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            flatten = tmp;
            flat_cap = new_cap;
        }
        memcpy(flatten + flat_len, row, curr_len * sizeof(seq_t));
        flat_len += curr_len;

        /* Clean up temporary structures */
        free(row);
        free(cnt);
        free(offset);
    }
}

/* Public API: returns the n‑th term of sequence A356784. */
seq_t a(idx_t n)
{
    _ensure_rows_up_to(n);
    return flatten[(size_t)n];
}
