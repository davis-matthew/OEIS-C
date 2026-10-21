/*
 * A274080  Table read by rows: row n gives all numbers less than n in the
 *          same row, column, or diagonal as n in the natural numbers read by
 *          antidiagonals.
 * https://oeis.org/A274080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 6237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_flat, get_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A274080:  Flattened list of numbers less than k that lie in the same
   row, column, or either diagonal of the natural numbers arranged by
   antidiagonals.  Computed by building the sets Si(k) for k=1,2,… and
   concatenating them in order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_flat.h"
#include "../utils/get_coords.h"

/* Compute a(n).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0 */
    /* static cache that is filled lazily */
    static seq_t *flattened = NULL;
    static size_t flattened_len = 0;
    static size_t flattened_cap = 0;
    static idx_t processed_upto = 0;

    while (flattened_len < n) {
        ++processed_upto;
        if (processed_upto <= 1) continue;            /* S(1) is empty */

        /* Boolean array marking numbers < processed_upto that belong to S(k) */
        bool *present = (bool *)calloc((size_t)processed_upto, sizeof(bool));
        if (!present) abort();

        idx_t i, j;
        get_coords(processed_upto, &i, &j);

        /* Row i */
        for (idx_t j_cur = 1; ; ++j_cur) {
            idx_t val = ((idx_t)(i + j_cur - 2)) * (i + j_cur - 1) / 2 + i;
            if (val >= processed_upto) break;
            present[val - 1] = true;
        }

        /* Column j */
        for (idx_t i_cur = 1; ; ++i_cur) {
            idx_t val = ((idx_t)(i_cur + j - 2)) * (i_cur + j - 1) / 2 + i_cur;
            if (val >= processed_upto) break;
            present[val - 1] = true;
        }

        /* Main diagonal (constant i - j) */
        int64_t diff = (int64_t)i - (int64_t)j;
        idx_t start_i = diff > 0 ? (idx_t)(diff + 1) : (idx_t)1;
        for (idx_t i_cur = start_i; ; ++i_cur) {
            idx_t j_cur = (idx_t)((int64_t)i_cur - diff);
            idx_t val = ((idx_t)(i_cur + j_cur - 2)) * (i_cur + j_cur - 1) / 2 + i_cur;
            if (val >= processed_upto) break;
            present[val - 1] = true;
        }

        /* Anti‑diagonal (constant i + j) */
        idx_t p = i + j;
        idx_t baseTri = (p >= 2) ? ((p - 2) * (p - 1)) / 2 : 0;
        for (idx_t i_cur = 1; i_cur < p; ++i_cur) {
            idx_t val = baseTri + i_cur;
            if (val >= processed_upto) break;
            present[val - 1] = true;
        }

        /* Append the sorted numbers < processed_upto to the global list */
        for (idx_t m = 1; m < processed_upto; ++m) {
            if (present[m - 1])
                append_flat(&flattened, &flattened_cap, &flattened_len, (seq_t)m);
        }

        free(present);
    }

    return flattened[n - 1];
}
