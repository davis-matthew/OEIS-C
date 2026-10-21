/*
 * A251216  Number of (n+1)X(5+1) 0..1 arrays with no 2X2 subblock having zero
 *          or two 1s.
 * https://oeis.org/A251216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: computed by dynamic programming over rows 
   using a transfer matrix for the 6‑column binary arrays.
   a(n) counts (n+1)×6 0..1 arrays with no 2×2 subblock having
   zero or two 1’s.  No closed form is known. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Transition matrix: allowed[a][b] is 1 iff rows a→b are compatible. */
    static unsigned char allowed[64][64] = {0};
    static bool init = false;
    if (!init) {
        init = true;
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                bool ok = true;
                for (int j = 0; j < 5 && ok; ++j) {
                    int w = ((a >> j)     & 1) +
                            ((a >> (j + 1)) & 1) +
                            ((b >> j)     & 1) +
                            ((b >> (j + 1)) & 1);
                    if (w == 0 || w == 2)   /* forbidden 2×2 block */
                        ok = false;
                }
                allowed[a][b] = ok ? 1 : 0;
            }
        }
    }

    /* DP arrays: dp[row_state] = number of ways to reach this state at current row. */
    seq_t dp[64], ndp[64];
    for (int i = 0; i < 64; ++i)
        dp[i] = (seq_t)1;                 /* first row can be anything */

    for (idx_t step = 0; step < n; ++step)   /* n transitions → n+1 rows */
    {
        for (int i = 0; i < 64; ++i)
            ndp[i] = (seq_t)0;

        for (int a = 0; a < 64; ++a) {
            seq_t val = dp[a];
            if (val == (seq_t)0)
                continue;
            for (int b = 0; b < 64; ++b)
                if (allowed[a][b])
                    ndp[b] += val;
        }

        /* move ndp → dp for next iteration */
        for (int i = 0; i < 64; ++i)
            dp[i] = ndp[i];
    }

    /* Sum over all final states */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 64; ++i)
        result += dp[i];

    return result;
}
