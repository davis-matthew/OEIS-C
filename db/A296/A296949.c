/*
 * A296949  Number of nX5 0..1 arrays with each 1 adjacent to 1, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = number of n×5 binary arrays where each 1 has 1, 3 or 5 king‑move
 * neighbors that are 1.  Computation is by dynamic programming over
 * pairs of consecutive rows.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static unsigned char VALID[32][32][32];
static unsigned char FINAL_VALID[32][32];
static unsigned char NEXT_COUNT[32][32];
static unsigned char NEXT_LIST[32][32][32];
static bool _init_done = false;

/* check whether the middle row satisfies the adjacency rule */
static bool valid_triple(int prev, int mid, int next)
{
    for (int c = 0; c < 5; ++c) {
        if (!((mid >> c) & 1)) continue;
        int cnt = 0;
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nc = c + dc;
                if (nc < 0 || nc > 4) continue;
                if (dr == -1) {
                    if ((prev >> nc) & 1) ++cnt;
                } else if (dr == 0) {
                    if ((mid >> nc) & 1) ++cnt;
                } else {
                    if ((next >> nc) & 1) ++cnt;
                }
            }
        }
        if (cnt != 1 && cnt != 3 && cnt != 5) return false;
    }
    return true;
}

/* pre‑compute all tables once */
static void init_tables(void)
{
    for (int prev = 0; prev < 32; ++prev) {
        for (int mid = 0; mid < 32; ++mid) {
            for (int next = 0; next < 32; ++next) {
                bool ok = valid_triple(prev, mid, next);
                VALID[prev][mid][next] = ok ? 1 : 0;
                if (next == 0) FINAL_VALID[prev][mid] = ok ? 1 : 0;
                if (ok) {
                    int idx = NEXT_COUNT[prev][mid];
                    NEXT_LIST[prev][mid][idx] = (unsigned char)next;
                    NEXT_COUNT[prev][mid] = idx + 1;
                }
            }
        }
    }
    _init_done = true;
}

static seq_t DP_CUR[32][32];
static seq_t DP_NEXT[32][32];

seq_t a(idx_t n)
{
    if (!_init_done) init_tables();

    if (n == 1) {
        seq_t a1 = 0;
        for (int m = 0; m < 32; ++m) {
            if (VALID[0][m][0]) a1 += 1;
        }
        return a1;
    }

    /* initialise DP for sequences of length 2: DP_CUR[prev][curr] = 1 if VALID[0][prev][curr] */
    memset(DP_CUR, 0, sizeof(DP_CUR));
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (VALID[0][prev][curr]) DP_CUR[prev][curr] = 1;
        }
    }

    if (n == 2) {
        seq_t val = 0;
        for (int p = 0; p < 32; ++p) {
            for (int c = 0; c < 32; ++c) {
                if (FINAL_VALID[p][c]) val += DP_CUR[p][c];
            }
        }
        return val;
    }

    /* propagate for rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(DP_NEXT, 0, sizeof(DP_NEXT));
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = DP_CUR[prev][curr];
                if (ways == 0) continue;
                unsigned char cnt = NEXT_COUNT[prev][curr];
                for (int k = 0; k < cnt; ++k) {
                    int next = NEXT_LIST[prev][curr][k];
                    DP_NEXT[curr][next] += ways;
                }
            }
        }
        memcpy(DP_CUR, DP_NEXT, sizeof(DP_CUR));
    }

    /* final validation with next row = 0 */
    seq_t val = 0;
    for (int p = 0; p < 32; ++p) {
        for (int c = 0; c < 32; ++c) {
            if (FINAL_VALID[p][c]) val += DP_CUR[p][c];
        }
    }
    return val;
}
