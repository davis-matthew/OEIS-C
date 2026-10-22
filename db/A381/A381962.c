/*
 * A381962  Irregular triangle read by rows, where row n lists the iterates of
 *          f(x), starting at x = n until f(x) <= 1, where f(x) is the Hamming
 *          weight of x (A000120).
 * https://oeis.org/A381962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base, easy
 * Formula:   a(n) = popcnt⁽ᵒᶠᶠᵗ⁾(row) where * row = i such that cum_sum[i‑1]
 *            ≤ n < cum_sum[i] * offset = n – cum_sum[i‑1] * popcnt⁽ᵒᶠᶠ⁾(x) is
 *            the Hamming weight applied offset times to x. * cum_sum[i] =
 *            Σ_{t=0..i} iter_len(t), iter_len(t) is the number of *
 *            iterations of the popcount on t until ≤1
 * Range:     u8 n <= 912, u16 n <= 9703, u32 n <= 9703, u64 n <= 9703,
 *            u128 n <= 9703, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Popcount on a 64‑bit value. */
static inline idx_t popcnt(idx_t x) {
    return (idx_t)__builtin_popcountll((unsigned long long)x);
}

/* Number of iterations needed for a starting value to reach ≤1. */
static inline idx_t iter_len(idx_t start) {
    if (start <= 1) return (idx_t)1;
    idx_t len = 1;
    while (start > 1) {
        start = popcnt(start);
        ++len;
    }
    return len;
}

/* Dynamic array of cumulative row lengths. */
static idx_t *cum_sum   = NULL;   /* cum_sum[i] = Σ_{t=0..i} iter_len(t) */
static size_t cum_cap   = 0;      /* allocated capacity of cum_sum   */
static idx_t  last_row  = 0;      /* last row that has been computed */

/* Ensure cum_sum covers all rows up to the smallest r with cum_sum[r] > target. */
static void ensure_cumulative(idx_t target) {
    if (!cum_sum) {
        cum_cap = 16;
        cum_sum = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        if (!cum_sum) exit(EXIT_FAILURE);
        cum_sum[0] = iter_len((idx_t)0);   /* row 0 length (always 1) */
        last_row = 0;
    }
    /* Extend cum_sum until it exceeds target. */
    while (cum_sum[last_row] <= target) {
        idx_t next_row = last_row + 1;
        idx_t len_next = iter_len(next_row);
        idx_t next_sum = cum_sum[last_row] + len_next;

        /* Grow array if needed. */
        if ((size_t)(last_row + 1) >= cum_cap) {
            size_t new_cap = cum_cap * 2;
            idx_t *tmp = (idx_t *)realloc(cum_sum, new_cap * sizeof(idx_t));
            if (!tmp) exit(EXIT_FAILURE);
            cum_sum = tmp;
            cum_cap = new_cap;
        }
        cum_sum[last_row + 1] = next_sum;
        ++last_row;
    }
}

/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n) {
    /* Locate the row containing index n. */
    ensure_cumulative(n);

    /* After ensure_cumulative, cum_sum[last_row] > n. */
    idx_t row        = last_row;                /* row index        */
    idx_t row_start  = (row == 0) ? 0 : cum_sum[row - 1];
    idx_t offset     = n - row_start;           /* position in row  */

    /* Start at 'row' and apply popcnt 'offset' times. */
    idx_t val = row;
    while (offset--) {
        val = popcnt(val);
    }

    return (seq_t)val;
}
