/*
 * A298458  Number of nX5 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays where each cell has between 2
 *            and 6 same-valued king‑move adjacent neighbours, the upper‑left
 *            element is 0. The value is computed by dynamic programming over
 *            rows with pre‑computed transition tables
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   valid_cells, valid_first_row, valid_last_row, valid_middle_row,
 *            valid_single_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/valid_cells.h"
#include "../utils/valid_first_row.h"
#include "../utils/valid_last_row.h"
#include "../utils/valid_middle_row.h"
#include "../utils/valid_single_row.h"

/* Pre‑computed tables (static to avoid re‑computation). */
static struct {
    bool ready;
    unsigned char next_cnt[32 * 32];               /* number of valid below rows */
    unsigned char next_rows[32 * 32][32];          /* list of valid below rows */
    bool last_valid[32 * 32];                      /* cur row is valid last */
    bool first_valid[32][32];                      /* first row judged with below */
} tables = { .ready = false };

static void precompute_tables(void)
{
    for (int above = 0; above < 32; ++above) {
        for (int cur = 0; cur < 32; ++cur) {
            int state = (above << 5) | cur;

            /* last row validity (no below) */
            tables.last_valid[state] = valid_last_row(above, cur);

            /* middle rows transition */
            unsigned char k = 0;
            for (int below = 0; below < 32; ++below) {
                if (valid_middle_row(above, cur, below)) {
                    tables.next_rows[state][k++] = (unsigned char)below;
                }
            }
            tables.next_cnt[state] = k;

            /* first row validity (no above) */
            if (above & 1) { /* upper left must be 0 for first row */
                tables.first_valid[above][cur] = false;
            } else {
                tables.first_valid[above][cur] = valid_first_row(above, cur);
            }
        }
    }
    tables.ready = true;
}

seq_t a(idx_t n)
{
    if (!tables.ready) precompute_tables();

    /* Special case n = 1 */
    if (n == 1) {
        seq_t res = 0;
        for (int r = 0; r < 32; ++r) {
            if ((r & 1) == 0 && valid_single_row(r)) {
                ++res;
            }
        }
        return res;
    }

    /* DP arrays: dp[prev][cur] */
    static seq_t dp[32][32];
    static seq_t newdp[32][32];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* Initialise with first two rows */
    for (int r0 = 0; r0 < 32; ++r0) {
        if ((r0 & 1) != 0) continue;           /* upper‑left must be 0 */
        for (int r1 = 0; r1 < 32; ++r1) {
            if (tables.first_valid[r0][r1]) {
                dp[r0][r1] = 1;
            }
        }
    }

    /* Process rows 2 .. n-1 (index i = 2 … n-1) */
    for (idx_t i = 2; i <= n - 1; ++i) {
        /* clear newdp */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                newdp[a][b] = 0;

        for (int prev_prev = 0; prev_prev < 32; ++prev_prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t ways = dp[prev_prev][cur];
                if (!ways) continue;
                int state = (prev_prev << 5) | cur;
                unsigned char cnt = tables.next_cnt[state];
                for (unsigned char k = 0; k < cnt; ++k) {
                    int below = tables.next_rows[state][k];
                    newdp[cur][below] += ways;
                }
            }
        }

        /* copy newdp to dp */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp[a][b] = newdp[a][b];
    }

    /* Final check: last row validity */
    seq_t result = 0;
    for (int above = 0; above < 32; ++above) {
        for (int cur = 0; cur < 32; ++cur) {
            seq_t ways = dp[above][cur];
            if (!ways) continue;
            if (tables.last_valid[(above << 5) | cur]) {
                result += ways;
            }
        }
    }
    return result;
}
