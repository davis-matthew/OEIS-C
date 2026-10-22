/*
 * A367634  Array read by descending antidiagonals, where row n=0 lists the
 *          natural numbers and where each new row n=1,2,... is found by
 *          taking the number n in the previous row, and "leaping" it over the
 *          next n terms to its right, keeping the other numbers fixed (see
 *          example).
 * https://oeis.org/A367634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Computes a(n) for OEIS A367634 by simulating the row‑by‑row
 *            construction of the array and extracting the element read by
 *            descending antidiagonals
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* n >= 1 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* undefined, guard for safety */

    /* find diagonal d such that d*(d+1)/2 < n <= (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (((unsigned __int128)(d + 1) * (d + 2)) / 2 < n) ++d;

    /* offset within diagonal d (0‑based) */
    idx_t cum_before = d * (d + 1) / 2;
    idx_t offset = n - cum_before - 1;
    idx_t row = offset;             /* 0‑based row index inside diagonal */
    idx_t col = d - row;            /* column index for this diagonal */

    /* length of each row we need to store */
    size_t L = (size_t)(d + 1);

    /* allocate and initialise row 0 */
    seq_t *prev = (seq_t *)calloc(L, sizeof(seq_t));
    if (!prev) return (seq_t)0;
    for (size_t j = 0; j < L; ++j) prev[j] = (seq_t)(j + 1);

    /* if we only need row 0, return directly */
    if (row == 0) {
        seq_t res = prev[col];
        free(prev);
        return res;
    }

    /* row buffer for building next rows */
    seq_t *cur = (seq_t *)calloc(L, sizeof(seq_t));
    if (!cur) { free(prev); return (seq_t)0; }

    /* build rows 1 .. row */
    for (idx_t i = 1; i <= row; ++i) {
        /* copy current row to temporary buffer */
        for (size_t j = 0; j < L; ++j) cur[j] = prev[j];

        /* find position of number i in current row */
        idx_t pos = L;  /* sentinel for "not found" */
        for (idx_t j = 0; j < L; ++j) {
            if (cur[j] == (seq_t)i) { pos = j; break; }
        }

        if (pos < L) {
            /* removal: shift left starting at pos */
            for (size_t j = pos; j + 1 < L; ++j) cur[j] = cur[j + 1];

            /* insertion index (may exceed the stored range) */
            idx_t target = pos + i;
            if (target <= L - 2) {
                /* shift right from target to the end of the valid region */
                for (idx_t j = L - 2; j >= target; --j)
                    cur[j + 1] = cur[j];
                cur[target] = (seq_t)i;
            } else if (target == L - 1) {
                cur[L - 1] = (seq_t)i;
            }   /* otherwise target > L-1: insertion beyond storage, ignore */
        }

        /* swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* prev now holds the desired row */
    seq_t result = prev[col];
    free(prev);
    free(cur);
    return result;
}
