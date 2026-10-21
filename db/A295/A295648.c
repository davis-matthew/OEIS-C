/*
 * A295648  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1, 3 or 4 1s.
 * https://oeis.org/A295648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   compute_row_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A295648
 * Formula: a(n) is the number of n×4 binary arrays where each 1 is adjacent
 * horizontally or vertically to 0, 1, 3 or 4 other 1s.
 * Computed by a linear transfer‑matrix DP.
 * Initial values: 13, 106, 956, 8391, 74335, 657178, ...
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_row_valid.h"

/* Pre‑computed tables for 4‑bit rows */
static bool row_valid[16][16][16];
static bool valid_transition[16][16][16];
static bool tables_ready = false;

/* Build the tables once (thread‑safe for a single invocation). */
static void build_tables(void)
{
    if (tables_ready) return;
    for (int r = 0; r < 16; ++r)
        for (int up = 0; up < 16; ++up)
            for (int down = 0; down < 16; ++down)
                row_valid[r][up][down] = compute_row_valid((uint8_t)r,
                                                          (uint8_t)up,
                                                          (uint8_t)down);
    for (int up = 0; up < 16; ++up)
        for (int curr = 0; curr < 16; ++curr)
            for (int next = 0; next < 16; ++next)
                valid_transition[up][curr][next] =
                    row_valid[curr][up][next];
    tables_ready = true;
}

seq_t a(idx_t n)
{
    build_tables();

    if (n == 0) return 0;          /* offset starts at 1 */
    if (n == 1) {
        seq_t sum = 0;
        for (uint8_t r = 0; r < 16; ++r)
            if (row_valid[r][0][0]) sum += 1;
        return sum;
    }

    /* DP over pairs of consecutive rows: state (prev, curr) */
    static seq_t dp[16][16];
    static seq_t newdp[16][16];

    /* initialize: up row = 0, current row can be any pattern */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    for (uint8_t curr = 0; curr < 16; ++curr)
        dp[0][curr] = 1;

    for (idx_t r = 2; r <= n; ++r) {
        /* clear newdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = 0;
        /* transition */
        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int next = 0; next < 16; ++next) {
                    if (valid_transition[prev][curr][next])
                        newdp[curr][next] += ways;
                }
            }
        }
        /* copy back */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* final row must be valid with down = 0 */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (dp[prev][curr] && row_valid[curr][prev][0])
                ans += dp[prev][curr];
        }
    }
    return ans;
}
