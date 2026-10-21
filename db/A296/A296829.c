/*
 * A296829  Number of nX3 0..1 arrays with each 1 adjacent to 0, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296829: dynamic‑programming count of n×3 binary arrays
   for which every 1 has 0, 3 or 5 king‑move adjacent 1s. */

#include <stdbool.h>

static bool seq_ok[8][8][8];
static bool seq_ok_computed = false;

static void init_ok(void)
{
    for (int up = 0; up < 8; ++up)
        for (int row = 0; row < 8; ++row)
            for (int down = 0; down < 8; ++down) {
                bool good = true;
                for (int c = 0; c < 3; ++c) {
                    if (!(row & (1 << c)))
                        continue;
                    int ncnt = 0;
                    /* up‑left, up, up‑right */
                    if (c > 0)
                        ncnt += (up >> (c - 1)) & 1;
                    ncnt += (up >> c) & 1;
                    if (c < 2)
                        ncnt += (up >> (c + 1)) & 1;
                    /* left/right in the same row */
                    if (c > 0)
                        ncnt += (row >> (c - 1)) & 1;
                    if (c < 2)
                        ncnt += (row >> (c + 1)) & 1;
                    /* down‑left, down, down‑right */
                    if (c > 0)
                        ncnt += (down >> (c - 1)) & 1;
                    ncnt += (down >> c) & 1;
                    if (c < 2)
                        ncnt += (down >> (c + 1)) & 1;
                    if (ncnt != 0 && ncnt != 3 && ncnt != 5) {
                        good = false;
                        break;
                    }
                }
                seq_ok[up][row][down] = good;
            }
    seq_ok_computed = true;
}

seq_t a(idx_t n)
{
    /* A296829 is defined for n ≥ 1; for n = 0 return 0. */
    if (n == 0)
        return 0;

    if (!seq_ok_computed)
        init_ok();

    /* DP arrays: state = (prev2 << 3) | prev1, 64 states. */
    static seq_t dp[64];
    static seq_t newdp[64];

    /* Initialize: previous two rows are zeros. */
    for (int i = 0; i < 64; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t i = 0; i < n; ++i) {
        for (int j = 0; j < 64; ++j)
            newdp[j] = 0;
        for (int state = 0; state < 64; ++state) {
            seq_t count = dp[state];
            if (count == 0)
                continue;
            int prev2 = (state >> 3) & 7;
            int prev1 = state & 7;
            for (int curr = 0; curr < 8; ++curr) {
                if (!seq_ok[prev2][prev1][curr])
                    continue;
                int newstate = (prev1 << 3) | curr;
                newdp[newstate] += count;
            }
        }
        /* copy newdp back to dp */
        for (int j = 0; j < 64; ++j)
            dp[j] = newdp[j];
    }

    /* After all rows processed, check final row against a bottom row of zeros. */
    seq_t total = 0;
    for (int state = 0; state < 64; ++state) {
        int prev2 = (state >> 3) & 7;
        int prev1 = state & 7;
        if (!seq_ok[prev2][prev1][0])
            continue;
        total += dp[state];
    }
    return total;
}
