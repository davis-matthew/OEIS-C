/*
 * A296721  Number of nX4 0..1 arrays with each 1 adjacent to 0, 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296721: Number of n×4 0..1 arrays with each 1 adjacent to
 * 0, 2 or 4 king‑move neighboring 1s.
 * This function computes the value by dynamic programming over rows.
 */

#include <stdbool.h>
#include <string.h>

/* check if a row *row* satisfies the adjacency constraint
 * when the row above is *prev* and the row below *next*.
 * All arguments are 4‑bit patterns (0..15). */
static bool valid_row(int row, int prev, int next)
{
    for (int col = 0; col < 4; ++col) {
        if (!(row & (1 << col)))
            continue;                 /* no constraint for 0s */

        int cnt = 0;

        /* same row neighbours */
        if (col > 0 && (row & (1 << (col - 1)))) cnt++;
        if (col < 3 && (row & (1 << (col + 1)))) cnt++;

        /* upper row neighbours */
        if (col > 0 && (prev & (1 << (col - 1)))) cnt++;
        if (prev & (1 << col))                      cnt++;
        if (col < 3 && (prev & (1 << (col + 1)))) cnt++;

        /* lower row neighbours */
        if (col > 0 && (next & (1 << (col - 1)))) cnt++;
        if (next & (1 << col))                     cnt++;
        if (col < 3 && (next & (1 << (col + 1)))) cnt++;

        if (cnt != 0 && cnt != 2 && cnt != 4)
            return false;
    }
    return true;
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1 */

    /* DP arrays: state is (prev_row, cur_row). */
    static seq_t dp[16][16];
    static seq_t newdp[16][16];
    seq_t result = 0;

    /* Initialise for the first row: prev row is 0 (non‑existent). */
    memset(dp, 0, sizeof(dp));
    for (int cur = 0; cur < 16; ++cur)
        dp[0][cur] = 1;

    /* Process rows 2..n */
    for (int r = 2; r <= (int)n; ++r) {
        memset(newdp, 0, sizeof(newdp));
        for (int prev = 0; prev < 16; ++prev) {
            for (int cur = 0; cur < 16; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways)
                    continue;
                for (int nxt = 0; nxt < 16; ++nxt) {
                    if (!valid_row(cur, prev, nxt))
                        continue;
                    newdp[cur][nxt] += ways;
                }
            }
        }
        memcpy(dp, newdp, sizeof(dp));
    }

    /* After the final row, validate it against prev row and an empty next row. */
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways)
                continue;
            if (valid_row(cur, prev, 0))
                result += ways;
        }
    }

    return result;
}
