/*
 * A358090  Partial inventory of positions as an irregular table; rows 1 and 2
 *          contain 1, for n > 2, row n contains the 1-based positions of 1's,
 *          followed by the positions of 2's, 3's, etc. in rows n-2 and n-1
 *          flattened.
 * https://oeis.org/A358090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 624, u16 n <= 9939, u32 n <= 9939, u64 n <= 9939,
 *            u128 n <= 9939, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the flattened table of rows where
   rows 1 and 2 are [1] and for n > 2, row n contains
   the 1‑based positions of 1, 2, 3, … in rows n-2 and n-1 flattened. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *flat = NULL;
    static size_t flat_cap = 0;
    static size_t flat_len = 0;

    static seq_t *row_pp = NULL;   /* row n-2   */
    static size_t row_pp_len = 0;

    static seq_t *row_prev = NULL; /* row n-1   */
    static size_t row_prev_len = 0;

    if (n == 0)
        return 0;               /* offset is 1, so this is unreachable in valid calls */

    /* lazy initialization for first two rows */
    if (flat_len == 0) {
        row_pp = (seq_t *)malloc(sizeof(seq_t));
        row_pp_len = 1;
        row_pp[0] = 1;

        row_prev = (seq_t *)malloc(sizeof(seq_t));
        row_prev_len = 1;
        row_prev[0] = 1;

        flat_cap = 2;
        flat = (seq_t *)malloc(flat_cap * sizeof(seq_t));
        flat_len = 0;
        flat[flat_len++] = 1; /* a(1) */
        flat[flat_len++] = 1; /* a(2) */
    }

    /* expand the table until we have at least n terms */
    while (flat_len < (size_t)n) {
        size_t len_next = row_pp_len + row_prev_len;

        /* frequency of each value in the flattened previous two rows */
        size_t *freq = (size_t *)calloc(len_next + 1, sizeof(size_t));
        if (!freq) return 0; /* allocation failure */

        for (size_t i = 0; i < row_pp_len; ++i) {
            size_t v = (size_t)row_pp[i];
            if (v <= len_next) freq[v]++;
        }
        for (size_t i = 0; i < row_prev_len; ++i) {
            size_t v = (size_t)row_prev[i];
            if (v <= len_next) freq[v]++;
        }

        /* starting index for each value in row_next */
        size_t *pos_start = (size_t *)malloc((len_next + 1) * sizeof(size_t));
        size_t cum = 0;
        for (size_t k = 1; k <= len_next; ++k) {
            pos_start[k] = cum;
            cum += freq[k];
        }

        /* mutable copy to fill positions */
        size_t *pos_filled = (size_t *)malloc((len_next + 1) * sizeof(size_t));
        for (size_t k = 1; k <= len_next; ++k) pos_filled[k] = pos_start[k];

        /* array to hold positions of flattened sequence */
        seq_t *pos_array = (seq_t *)malloc(len_next * sizeof(seq_t));

        /* populate pos_array with 1‑based positions of each entry */
        size_t idx_flat = 0;
        for (size_t i = 0; i < row_pp_len; ++i) {
            size_t v = (size_t)row_pp[i];
            pos_array[pos_filled[v]++] = (seq_t)(idx_flat + 1);
            ++idx_flat;
        }
        for (size_t i = 0; i < row_prev_len; ++i) {
            size_t v = (size_t)row_prev[i];
            pos_array[pos_filled[v]++] = (seq_t)(idx_flat + 1);
            ++idx_flat;
        }

        /* build new row by concatenating sorted groups */
        seq_t *row_next = (seq_t *)malloc(len_next * sizeof(seq_t));
        size_t out_idx = 0;
        for (size_t k = 1; k <= len_next; ++k) {
            size_t start = pos_start[k];
            size_t end = start + freq[k];
            for (size_t p = start; p < end; ++p) {
                row_next[out_idx++] = pos_array[p];
            }
        }

        /* free temporary arrays */
        free(freq);
        free(pos_start);
        free(pos_filled);
        free(pos_array);

        /* extend the flattened table */
        if (flat_cap < flat_len + len_next) {
            size_t new_cap = flat_cap ? flat_cap : 16;
            while (new_cap < flat_len + len_next) new_cap <<= 1;
            flat = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
            flat_cap = new_cap;
        }
        for (size_t i = 0; i < len_next; ++i) {
            flat[flat_len++] = row_next[i];
        }

        /* prepare for next iteration */
        free(row_pp);                     /* old oldest row */
        row_pp = row_prev;
        row_pp_len = row_prev_len;
        row_prev = row_next;
        row_prev_len = len_next;
    }

    return flat[n - 1];
}
