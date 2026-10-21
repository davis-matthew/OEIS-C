/*
 * A297370  Number of n X 4 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0, 1 or 2 neighboring 1's.
 * https://oeis.org/A297370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements computation of OEIS A297370:
   Number of n×4 binary arrays where each 1 has at most 2 neighbors
   among horizontally adjacent and antidiagonally adjacent cells.
   The DP uses states representing the last two rows.
*/
#include <stdbool.h>

static bool _trans_ok[16][16][16];
static bool _final_ok[16][16];
static bool _tables_init = false;

static bool check_row(int row, int above, int below) {
    for (int j = 0; j < 4; ++j) {
        if ((row >> j) & 1) {
            int count = 0;
            if (j > 0 && ((row >> (j - 1)) & 1)) ++count;
            if (j < 3 && ((row >> (j + 1)) & 1)) ++count;
            if (j < 3 && ((above >> (j + 1)) & 1)) ++count;
            if (j > 0 && ((below >> (j - 1)) & 1)) ++count;
            if (count > 2) return false;
        }
    }
    return true;
}

static void init_tables(void) {
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            _final_ok[a][b] = check_row(b, a, 0);
            for (int c = 0; c < 16; ++c) {
                _trans_ok[a][b][c] = check_row(b, a, c);
            }
        }
    }
    _tables_init = true;
}

seq_t a(idx_t n) {
    if (!_tables_init) init_tables();

    /* DP tables: dp[prev_row][current_row] */
    seq_t dp[16][16];
    seq_t ndp[16][16];
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    dp[0][0] = (seq_t)1; /* start with virtual row of zeros */

    for (idx_t i = 1; i <= n; ++i) {
        for (int p = 0; p < 16; ++p)
            for (int q = 0; q < 16; ++q)
                ndp[p][q] = 0;

        for (int prev2 = 0; prev2 < 16; ++prev2) {
            for (int prev1 = 0; prev1 < 16; ++prev1) {
                seq_t ways = dp[prev2][prev1];
                if (!ways) continue;
                for (int cur = 0; cur < 16; ++cur) {
                    if (_trans_ok[prev2][prev1][cur]) {
                        ndp[prev1][cur] += ways;
                    }
                }
            }
        }

        /* copy ndp to dp */
        for (int p = 0; p < 16; ++p)
            for (int q = 0; q < 16; ++q)
                dp[p][q] = ndp[p][q];
    }

    /* Sum over final states, checking last row constraints */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            if (_final_ok[prev][cur]) {
                result += dp[prev][cur];
            }
        }
    }
    return result;
}
