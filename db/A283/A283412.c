/*
 * A283412  Number of n X 5 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A283412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with no 1 equal to more than
 *            three of its horizontal, vertical and antidiagonal neighbors
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define WIDTH 5
#define ROWS  (1U << WIDTH)      /* 32 possible rows */

/* Precompute for each (prev,curr) the list of rows 'next' that keep
   'curr' valid (neighbors of all 1’s in 'curr' <= 3). */
static uint8_t  next_rows[ROWS * ROWS][ROWS];
static uint8_t  next_rows_cnt[ROWS * ROWS];
static bool     next_rows_inited = false;

/* Helper: check whether the row 'curr' is valid given the rows
   'prev' (above) and 'next' (below).  Missing rows are considered all zeros. */
static bool is_valid(uint32_t prev, uint32_t curr, uint32_t next)
{
    for (uint32_t j = 0; j < WIDTH; ++j) {
        if (((curr >> j) & 1U) == 0) continue;          /* only check 1‑cells */

        uint32_t cnt = 0;
        /* horizontal neighbors */
        if (j > 0 && ((curr >> (j-1)) & 1U)) ++cnt;
        if (j + 1 < WIDTH && ((curr >> (j+1)) & 1U)) ++cnt;

        /* vertical neighbors */
        if ((prev >> j) & 1U) ++cnt;
        if ((next >> j) & 1U) ++cnt;

        /* antidiagonal neighbors */
        if (j + 1 < WIDTH && ((prev >> (j+1)) & 1U)) ++cnt;
        if (j > 0 && ((next >> (j-1)) & 1U)) ++cnt;

        if (cnt > 3) return false;
    }
    return true;
}

/* Build the precomputed lists once. */
static void init_next_rows(void)
{
    if (next_rows_inited) return;

    for (uint32_t prev = 0; prev < ROWS; ++prev) {
        for (uint32_t curr = 0; curr < ROWS; ++curr) {
            uint8_t *list   = next_rows[prev * ROWS + curr];
            uint8_t   cnt   = 0;
            for (uint32_t next = 0; next < ROWS; ++next) {
                if (is_valid(prev, curr, next)) {
                    list[cnt++] = (uint8_t)next;
                }
            }
            next_rows_cnt[prev * ROWS + curr] = cnt;
        }
    }
    next_rows_inited = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined as 0 (outside offset) */

    /* The DP tables.  Size 32×32 = 1024 entries.  Allowed as static
       because ROWS is a compile-time constant. */
    static seq_t dp[ROWS * ROWS];
    static seq_t newdp[ROWS * ROWS];

    init_next_rows();

    /* Zero out DP arrays */
    for (size_t i = 0; i < ROWS * ROWS; ++i) {
        dp[i] = (seq_t)0;
        newdp[i] = (seq_t)0;
    }

    /* Initial state after first row: prev = 0 (no row above) */
    for (uint32_t curr = 0; curr < ROWS; ++curr) {
        dp[0 * ROWS + curr] = (seq_t)1;
    }

    /* Iterate rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        for (size_t i = 0; i < ROWS * ROWS; ++i) newdp[i] = (seq_t)0;

        for (uint32_t prev = 0; prev < ROWS; ++prev) {
            for (uint32_t curr = 0; curr < ROWS; ++curr) {
                seq_t ways = dp[prev * ROWS + curr];
                if (ways == (seq_t)0) continue;

                uint8_t *list = next_rows[prev * ROWS + curr];
                uint8_t   cnt = next_rows_cnt[prev * ROWS + curr];

                for (uint8_t k = 0; k < cnt; ++k) {
                    uint32_t next = list[k];
                    newdp[curr * ROWS + next] += ways;
                }
            }
        }

        /* swap dp and newdp */
        for (size_t i = 0; i < ROWS * ROWS; ++i) dp[i] = newdp[i];
    }

    /* Add contributions from the final row (next = 0). */
    seq_t result = (seq_t)0;
    for (uint32_t prev = 0; prev < ROWS; ++prev) {
        for (uint32_t curr = 0; curr < ROWS; ++curr) {
            seq_t ways = dp[prev * ROWS + curr];
            if (ways == (seq_t)0) continue;
            if (is_valid(prev, curr, 0U)) {
                result += ways;
            }
        }
    }
    return result;
}
