/*
 * A296720  Number of n X 3 0..1 arrays with each 1 adjacent to 0, 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 0..1 arrays such that every 1 has 0, 2, or
 *            4 king-move neighboring 1s. Computed by a linear‑time DP over
 *            the 8 possible row patterns
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Transition table:  trans[pp][p][c] == 1 iff, after adding
       a row pattern 'c' after the pair (pp, p),
       every 1 in row 'p' has 0, 2, or 4 king‑move neighbors in
       rows pp / p / c.  It is built once. */
    static uint8_t trans[8][8][8];
    static int ready = 0;
    if (!ready) {
        for (int pp = 0; pp < 8; ++pp)
            for (int p = 0; p < 8; ++p)
                for (int c = 0; c < 8; ++c) {
                    int ok = 1;
                    for (int col = 0; col < 3 && ok; ++col) {
                        if (((p >> col) & 1) == 0) continue;
                        int cnt = 0;
                        for (int dr = -1; dr <= 1; ++dr)
                            for (int dc = -1; dc <= 1; ++dc) {
                                if (dr == 0 && dc == 0) continue;
                                int col2 = col + dc;
                                if (col2 < 0 || col2 > 2) continue;
                                int rowpat;
                                if (dr == -1)       rowpat = pp;
                                else if (dr == 0)   rowpat = p;
                                else                rowpat = c;
                                cnt += (rowpat >> col2) & 1;
                            }
                        if (!(cnt == 0 || cnt == 2 || cnt == 4)) ok = 0;
                    }
                    trans[pp][p][c] = ok;
                }
        ready = 1;
    }

    size_t NN = (size_t)n;                /* number of real rows */
    seq_t dp[8][8]  = {0}, next_dp[8][8] = {0};

    dp[0][0] = 1;                         /* start with two zero rows */

    for (size_t step = 0; step < NN + 1; ++step) { /* N real rows + final dummy row 0 */
        /* reset next_dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                next_dp[i][j] = 0;

        for (int pp = 0; pp < 8; ++pp)
            for (int p = 0; p < 8; ++p) {
                seq_t val = dp[pp][p];
                if (!val) continue;
                for (int c = 0; c < 8; ++c)
                    if (trans[pp][p][c])
                        next_dp[p][c] += val;
            }

        /* copy back to dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* Sum over all states where the last real row has been checked
       and the dummy final row is zero. */
    seq_t result = 0;
    for (int pp = 0; pp < 8; ++pp)
        result += dp[pp][0];

    return result;
}
