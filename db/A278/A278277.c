/*
 * A278277  Number of nX5 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) or (1,0), with upper left element zero.
 * https://oeis.org/A278277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   check_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A278277 – number of n×5 binary arrays with
    a(1) = 0 (upper‐left element 0),
    each element has both an equal and a not equal neighbor among
    offsets: (-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,0).   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/check_row.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;   /* no array satisfies the constraints */

    /* --- pre‑computed transition tables (computed once) --- */
    static bool ready = false;
    static bool first_ok[32][32];
    static bool mid_ok[32][32][32];
    static bool last_ok[32][32];

    if (!ready) {
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                /* first row: has_above=false, has_below=true */
                first_ok[i][j] = ((i & 1) == 0 &&
                                  check_row(-1, i, j, false, true));
            }
        }
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                for (int k = 0; k < 32; ++k) {
                    /* middle rows: has_above=true, has_below=true */
                    mid_ok[i][j][k] = check_row(i, j, k, true, true);
                }
            }
        }
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                /* last row: has_above=true, has_below=false */
                last_ok[i][j] = check_row(i, j, 0, true, false);
            }
        }
        ready = true;
    }

    /* --- DP tables --- */
    static seq_t dp[32][32];
    static seq_t ndp[32][32];

    /* reset dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = (seq_t)0;

    /* initial state: first two rows, after verifying row 0 */
    for (int i = 0; i < 32; ++i) {
        if ((i & 1) == 0) {      /* upper‑left zero constraint */
            for (int j = 0; j < 32; ++j) {
                if (first_ok[i][j])
                    dp[i][j] = (seq_t)1;
            }
        }
    }

    /* add rows 2 .. n-1 (0‑based row indices) */
    for (idx_t r = 2; r < n; ++r) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = (seq_t)0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (!cnt) continue;
                for (int next = 0; next < 32; ++next) {
                    if (mid_ok[prev][curr][next])
                        ndp[curr][next] += cnt;
                }
            }
        }

        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum over states where the last row satisfies the constraints */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (last_ok[prev][curr])
                ans += dp[prev][curr];
        }
    }
    return ans;
}
