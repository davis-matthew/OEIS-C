/*
 * A296631  Number of nX4 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A296631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays where every 1 has 0, 2 or 4
 *            neighbors among the horizontal, vertical and anti‑diagonal
 *            directions. Computed by dynamic programming over rows
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* INVALID if n == 0.  The sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;

    /* Precompute all-valid triples (prev, curr, next) once. */
    static uint8_t valid[16][16][16];
    static int valid_init = 0;
    if (!valid_init) {
        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                for (int next = 0; next < 16; ++next) {
                    int ok = 1;
                    for (int c = 0; c < 4; ++c) {
                        if (curr & (1 << c)) {
                            int cnt = 0;
                            /* horizontal neighbours */
                            if (c > 0     && (curr & (1 << (c - 1)))) ++cnt;
                            if (c < 3     && (curr & (1 << (c + 1)))) ++cnt;
                            /* vertical neighbours */
                            if (prev & (1 << c)) ++cnt;
                            if (next & (1 << c)) ++cnt;
                            /* anti‑diagonal neighbours */
                            if (c < 3     && (prev & (1 << (c + 1)))) ++cnt;
                            if (c > 0     && (next & (1 << (c - 1)))) ++cnt;
                            if (!(cnt == 0 || cnt == 2 || cnt == 4)) {
                                ok = 0;
                                break;
                            }
                        }
                    }
                    valid[prev][curr][next] = (uint8_t)ok;
                }
            }
        }
        valid_init = 1;
    }

    /* DP arrays: dp[prev][curr] holds the number of ways to build rows up to
       the current row with the last two rows being 'prev' and 'curr'. */
    seq_t dp[16][16];
    seq_t ndp[16][16];

    /* initialise dp[0][mask] = 1 for all first‑row masks;
       the empty virtual row above has mask 0. */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    for (int mask = 0; mask < 16; ++mask)
        dp[0][mask] = (seq_t)1;

    /* Add rows 2 .. n.  For each new row we iterate through all possible
       triples (prev, curr, next) that satisfy the constraints for 'curr'. */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                for (int next = 0; next < 16; ++next) {
                    if (!valid[prev][curr][next]) continue;
                    ndp[curr][next] += ways;
                }
            }
        }

        /* move ndp to dp for the next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final row has no next row: we need to check the constraints for
       the last row with 'next' == 0. */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            seq_t ways = dp[prev][curr];
            if (ways == 0) continue;
            if (!valid[prev][curr][0]) continue;
            result += ways;
        }
    }

    return result;
}
