/*
 * A375946  Expansion of e.g.f. 1 / (1 + 3 * log(1 - x))^(4/3).
 * https://oeis.org/A375946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A007559(k+1) * |Stirling1(n,k)|
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdlib.h>
#include <stddef.h>

static idx_t max_n = 0;
static seq_t *A_arr  = NULL; /* A_arr[k] = A007559(k+1)  */
static seq_t *a_arr  = NULL; /* a_arr[n]  */
static seq_t *S_rows[2] = {NULL, NULL}; /* two rows for Stirling(1) */
static uint8_t cur_row = 0;              /* which S_rows element holds the current row */

/* Reallocate all arrays to at least new_size (new_size >= current size) */
static void ensure_capacity(idx_t new_size) {
    if (new_size <= (idx_t)(S_rows[0] ? 0 : 0)) return; /* already enough */

    /* Reallocate A_arr */
    A_arr = (seq_t *)realloc(A_arr, new_size * sizeof(seq_t));
    if (!A_arr) abort();

    /* Reallocate a_arr */
    a_arr = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
    if (!a_arr) abort();

    /* Reallocate both Stirling rows */
    for (int i = 0; i < 2; ++i) {
        S_rows[i] = (seq_t *)realloc(S_rows[i], new_size * sizeof(seq_t));
        if (!S_rows[i]) abort();
    }

    /* No need to init new elements; they will be overwritten */
}

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n) {
    /* Initialize for n == 0 if not yet done */
    if (max_n == 0 && n == 0) {
        ensure_capacity(1);
        A_arr[0] = (seq_t)1;
        a_arr[0] = (seq_t)1;
        S_rows[0][0] = (seq_t)1;
        cur_row = 0;
        return a_arr[0];
    }

    /* Expand arrays if necessary */
    if (n > max_n) {
        idx_t new_size = n + 1;
        ensure_capacity(new_size);

        /* Prepare to recompute from max_n+1 up to n */
        uint8_t prev_row = cur_row;
        for (idx_t row = max_n + 1; row <= n; ++row) {
            /* Compute A007559(row+1) = (3*row+1) * A007559(row) */
            if (row >= 1) {
                seq_t factor = (seq_t)((idx_t)3 * row + 1);
                A_arr[row] = A_arr[row - 1] * factor;
            }

            /* Compute Stirling1(row, k) into S_rows[prev_row ^ 1] */
            seq_t *Sprev = S_rows[prev_row];
            seq_t *Scur  = S_rows[prev_row ^ 1];
            /* j = 0 */
            Scur[0] = (seq_t)0;
            /* 1 <= j < row */
            for (idx_t j = 1; j < row; ++j) {
                seq_t term = Sprev[j];
                term *= (seq_t)(row - 1);
                Scur[j] = Sprev[j - 1] + term;
            }
            /* j = row */
            Scur[row] = (seq_t)1;

            /* Compute a[row] = sum_{k=0..row} A_arr[k] * Scur[k] */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= row; ++k) {
                sum += A_arr[k] * Scur[k];
            }
            a_arr[row] = sum;

            /* Prepare for next iteration */
            prev_row ^= 1;
            cur_row = prev_row;
        }
        max_n = n;
    }

    return a_arr[n];
}
