/*
 * A281835  Number of n X 6 0..1 arrays with no element equal to more than
 *          four of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A281835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 6 binary arrays with first element 0 such
 *            that every cell has at most four equal king‑move neighbors
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define STATE_COUNT 4160   // 65 (sentinel + 64) * 64

static uint8_t  adj[STATE_COUNT][64];
static uint8_t  adj_cnt[STATE_COUNT];
static bool     last_ok[STATE_COUNT];
static bool     precomp = false;

/* Check whether the row `mid` is valid given upper row `upp`
   (if `have_upp` is true) and lower row `low` (if `have_low` is true). */
static bool valid(const int upp, const int mid, const int low,
                  bool have_upp, bool have_low)
{
    for (int j = 0; j < 6; ++j) {
        int v = (mid >> j) & 1;
        int same = 0;
        if (j > 0)           if (((mid >> (j-1)) & 1) == v) ++same;
        if (j < 5)           if (((mid >> (j+1)) & 1) == v) ++same;
        if (have_upp)        if (((upp >> j) & 1)          == v) ++same;
        if (have_upp) {
            if (j > 0)       if (((upp >> (j-1)) & 1)    == v) ++same;
            if (j < 5)       if (((upp >> (j+1)) & 1)    == v) ++same;
        }
        if (have_low)        if (((low >> j) & 1)          == v) ++same;
        if (have_low) {
            if (j > 0)       if (((low >> (j-1)) & 1)    == v) ++same;
            if (j < 5)       if (((low >> (j+1)) & 1)    == v) ++same;
        }
        if (same > 4) return false;
    }
    return true;
}

/* Precompute adjacency lists and last row validity. */
static void precompute(void)
{
    for (int upp = 0; upp <= 64; ++upp) {
        bool have_upp = (upp != 64);
        for (int mid = 0; mid < 64; ++mid) {
            int id = upp * 64 + mid;
            int cnt = 0;
            for (int low = 0; low < 64; ++low) {
                if (valid(upp, mid, low, have_upp, true)) {
                    adj[id][cnt++] = low;
                }
            }
            adj_cnt[id] = cnt;
            last_ok[id] = valid(upp, mid, 0, have_upp, false);
        }
    }
    precomp = true;
}

seq_t a(idx_t n)
{
    if (!precomp) precompute();

    static seq_t dp[STATE_COUNT];
    static seq_t ndp[STATE_COUNT];

    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;

    /* first row: first cell must be 0 */
    for (int r = 0; r < 64; ++r)
        if ((r & 1) == 0)
            dp[64 * 64 + r] = 1;

    for (idx_t row = 1; row < n; ++row) {
        memset(ndp, 0, sizeof(ndp));
        for (int id = 0; id < STATE_COUNT; ++id) {
            seq_t cnt = dp[id];
            if (!cnt) continue;
            uint8_t kcnt = adj_cnt[id];
            for (int k = 0; k < kcnt; ++k) {
                int low = adj[id][k];
                int new_id = (id % 64) * 64 + low;
                ndp[new_id] += cnt;
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    seq_t ans = 0;
    for (int id = 0; id < STATE_COUNT; ++id)
        if (dp[id] && last_ok[id])
            ans += dp[id];

    return ans;
}
