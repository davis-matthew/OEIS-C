/*
 * A295778  Number of nX5 0..1 arrays with each 1 adjacent to 0 or 2 king-move
 *          neighboring 1s.
 * https://oeis.org/A295778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over rows with 32 states (5-bit patterns). The recurrence *
 *            follows from the state transition matrix
 *            transition[row_curr][row_prev][row_next] * where a transition is
 *            allowed iff the middle row satisfies the * adjacency rule with
 *            its upper and lower neighbours. * Initial condition: the first
 *            ro...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static unsigned char transition[32][32][32];
static unsigned char final_valid[32][32];
static int precomp_done = 0;

/* Check if a row satisfies the rule given its upper and lower neighbours. */
static int is_valid_row(int row, int up, int down) {
    for (int j = 0; j < 5; ++j) {
        if (!(row >> j & 1))
            continue;
        int cnt = 0;
        for (int dr = -1; dr <= 1; ++dr) {
            int nbr = (dr == -1) ? up : (dr == 0) ? row : down;
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0)
                    continue;
                int c = j + dc;
                if (c < 0 || c >= 5)
                    continue;
                if (nbr >> c & 1)
                    ++cnt;
            }
        }
        if (cnt != 0 && cnt != 2)
            return 0;
    }
    return 1;
}

/* Precompute the transition tables once. */
static void precompute(void) {
    for (int up = 0; up < 32; ++up) {
        for (int curr = 0; curr < 32; ++curr) {
            for (int next = 0; next < 32; ++next) {
                transition[curr][up][next] = is_valid_row(curr, up, next);
            }
            final_valid[curr][up] = is_valid_row(curr, up, 0);
        }
    }
    precomp_done = 1;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (!precomp_done)
        precompute();

    if (n == 0) /* offset is 1; n==0 is unused */
        return (seq_t)0;

    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 32; ++r)
            if (final_valid[r][0])
                ans += (seq_t)1;
        return ans;
    }

    static seq_t dp_prev[32][32];
    static seq_t dp_curr[32][32];
    memset(dp_prev, 0, sizeof(dp_prev));
    /* First row with virtual row 0 */
    for (int r = 0; r < 32; ++r)
        dp_prev[r][0] = (seq_t)1;

    for (idx_t row = 2; row <= n; ++row) {
        memset(dp_curr, 0, sizeof(dp_curr));
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp_prev[curr][prev];
                if (val == 0)
                    continue;
                for (int next = 0; next < 32; ++next) {
                    if (transition[curr][prev][next]) {
                        dp_curr[next][curr] += val;
                    }
                }
            }
        }
        memcpy(dp_prev, dp_curr, sizeof(dp_prev));
    }

    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t val = dp_prev[curr][prev];
            if (val == 0)
                continue;
            if (final_valid[curr][prev])
                ans += val;
        }
    }
    return ans;
}
