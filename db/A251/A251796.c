/*
 * A251796  Number of (n+1)X(3+1) 0..3 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   init_adjacency
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251796: Number of (n+1)×4 arrays with entries in {0,1,2,3}
   such that for every 2×2 subblock the condition
   x_{i+1,j+1} - x_{i,j} ≥ x_{i+1,j} - x_{i,j+1}
   holds.  This is equivalent to
   a_{i+1}[j+1] + a_i[j+1] ≥ a_{i+1}[j] + a_i[j] for all i,j. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* for memset */
#include "../utils/init_adjacency.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* below offset, return 0 */
        return 0;

    static bool initialized = false;
    static unsigned char adj[256][256];
    static seq_t dp_cur[256];
    static seq_t dp_next[256];
    static idx_t current_rows = 1; /* number of rows represented by dp_cur */

    if (!initialized)
    {
        init_adjacency(adj);
        for (int i = 0; i < 256; ++i)
            dp_cur[i] = (seq_t)1;  /* one way to have a single row equal to i */
        memset(dp_next, 0, sizeof(dp_next));
        initialized = true;
    }

    /* Grow DP to length n+1 rows if needed */
    while (current_rows < n + 1)
    {
        /* dp_next[w] = sum over v: dp_cur[v] * adj[v][w] (adjacent) */
        for (int w = 0; w < 256; ++w)
            dp_next[w] = 0;

        for (int v = 0; v < 256; ++v)
        {
            seq_t val = dp_cur[v];
            if (val == 0) continue;
            unsigned char *row = adj[v];
            for (int w = 0; w < 256; ++w)
            {
                if (row[w])
                    dp_next[w] += val;
            }
        }

        /* move to next step */
        for (int i = 0; i < 256; ++i)
            dp_cur[i] = dp_next[i];

        ++current_rows;
    }

    /* Sum all dp_cur to obtain a(n) */
    seq_t ans = 0;
    for (int i = 0; i < 256; ++i)
        ans += dp_cur[i];
    return ans;
}
