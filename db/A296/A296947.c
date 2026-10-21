/*
 * A296947  Number of n X 3 0..1 arrays with each 1 adjacent to 1, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296947: Number of n × 3 binary arrays whose every 1 has
 * 1, 3, or 5 king‑move neighboring 1s.
 * Computed by dynamic programming over the last two rows of the
 * array.  The DP state consists of two 3‑bit rows (previous and
 * current).  For each new row we check that the middle row satisfies
 * the adjacency condition and transition to the new pair (current,
 * next).  After the last row the final row must also satisfy the
 * condition with the below row being all zeros. */
#include <stdbool.h>

static int validity[8][8][8] = {0};
static bool valid_initialized = false;

static void init_validity(void)
{
    for (int up = 0; up < 8; ++up) {
        for (int mid = 0; mid < 8; ++mid) {
            for (int down = 0; down < 8; ++down) {
                bool ok = true;
                for (int k = 0; k < 3; ++k) {
                    if ((mid >> k) & 1) {
                        unsigned neigh = 0;
                        /* upper row */
                        for (int c = k - 1; c <= k + 1; ++c) {
                            if (c >= 0 && c <= 2) {
                                if (up & (1 << c)) ++neigh;
                            }
                        }
                        /* same row neighbours (excluding self) */
                        if (k - 1 >= 0 && (mid & (1 << (k - 1)))) ++neigh;
                        if (k + 1 <= 2 && (mid & (1 << (k + 1)))) ++neigh;
                        /* lower row */
                        for (int c = k - 1; c <= k + 1; ++c) {
                            if (c >= 0 && c <= 2) {
                                if (down & (1 << c)) ++neigh;
                            }
                        }
                        if (!(neigh == 1 || neigh == 3 || neigh == 5)) {
                            ok = false;
                            break;
                        }
                    }
                }
                validity[up][mid][down] = ok;
            }
        }
    }
    valid_initialized = true;
}

seq_t a(idx_t n)
{
    if (!valid_initialized) init_validity();

    /* DP arrays: state index = prev*8 + curr (0..63) */
    seq_t dp[64] = {0};
    seq_t newdp[64] = {0};

    /* initial state: previous row is all zeros, current is any first row */
    for (int cur = 0; cur < 8; ++cur)
        dp[0 * 8 + cur] += (seq_t)1;

    /* iterate adding rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 64; ++i) newdp[i] = (seq_t)0;
        for (int prev = 0; prev < 8; ++prev) {
            for (int cur = 0; cur < 8; ++cur) {
                seq_t cnt = dp[prev * 8 + cur];
                if (cnt == (seq_t)0) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (validity[prev][cur][nxt]) {
                        newdp[cur * 8 + nxt] += cnt;
                    }
                }
            }
        }
        for (int i = 0; i < 64; ++i) dp[i] = newdp[i];
    }

    /* after processing n rows, validate the final row against zeros below */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int cur = 0; cur < 8; ++cur) {
            seq_t cnt = dp[prev * 8 + cur];
            if (cnt == (seq_t)0) continue;
            if (validity[prev][cur][0]) ans += cnt;
        }
    }
    return ans;
}
