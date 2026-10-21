/*
 * A251098  Number of (n+1)X(6+1) 0..1 arrays with no 2X2 subblock having its
 *          minimum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A251098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   adjacent_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over rows for binary arrays of size
   (n+1)×7 that avoid a 2×2 subblock whose minimum diagonal element is
   less than its minimum antidiagonal element. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/adjacent_ok.h"

static uint8_t adj_next[128][128];
static uint8_t adj_cnt[128];
static bool adj_init = false;

/* Build adjacency lists once. */
static void
build_adj (void)
{
    for (int p = 0; p < 128; ++p)
    {
        adj_cnt[p] = 0;
        for (int q = 0; q < 128; ++q)
        {
            if (adjacent_ok ((uint8_t)p, (uint8_t)q))
                adj_next[p][adj_cnt[p]++] = (uint8_t)q;
        }
    }
    adj_init = true;
}

/* Main sequence function. */
seq_t a (idx_t n)
{
    if (!adj_init)
        build_adj ();

    /* dp arrays: dp_prev is counts for the current number of rows
       processed; dp_next will hold the next state. */
    seq_t dp_prev[128];
    seq_t dp_next[128];

    /* Start with one row: any of the 128 patterns is allowed. */
    for (int i = 0; i < 128; ++i)
        dp_prev[i] = 1;

    /* We need n additional rows to reach (n+1) rows. */
    for (idx_t step = 1; step <= n; ++step)
    {
        /* zero next state */
        for (int i = 0; i < 128; ++i)
            dp_next[i] = 0;

        /* transition */
        for (int p = 0; p < 128; ++p)
        {
            seq_t val = dp_prev[p];
            if (val == 0)
                continue;
            uint8_t *nb = adj_next[p];
            uint8_t   nbc = adj_cnt[p];
            for (uint8_t k = 0; k < nbc; ++k)
                dp_next[nb[k]] += val;
        }

        /* swap dp_prev and dp_next */
        for (int i = 0; i < 128; ++i)
            dp_prev[i] = dp_next[i];
    }

    /* Sum over all final‑row patterns to obtain the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < 128; ++i)
        total += dp_prev[i];

    return total;
}
