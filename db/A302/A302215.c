/*
 * A302215  Number of 5Xn 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Transfer‐matrix DP for 5×n binary matrices with the rule
   that each cell has 0, 2, or 3 equal neighbours among the
   horizontally and diagonally adjacent cells (vertical neighbours
   are NOT included).  The upper‑left cell is fixed to 0. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Return true iff column `cur` satisfies the rule when it has
   neighbouring columns `left` (exists if left_exists==true)
   and `right` (exists if right_exists==true). */
static inline bool valid_col(uint32_t left, uint32_t cur, uint32_t right,
                             bool left_exists, bool right_exists)
{
    for (int r = 0; r < 5; ++r) {
        int cur_bit = (cur >> r) & 1;
        int cnt = 0;

        if (left_exists) {
            if (((left >> r) & 1) == cur_bit) ++cnt;           /* left */
            if (r > 0 && (((left >> (r - 1)) & 1) == cur_bit)) ++cnt; /* left‑up */
            if (r < 4 && (((left >> (r + 1)) & 1) == cur_bit)) ++cnt; /* left‑down */
        }
        if (right_exists) {
            if (((right >> r) & 1) == cur_bit) ++cnt;           /* right */
            if (r > 0 && (((right >> (r - 1)) & 1) == cur_bit)) ++cnt; /* right‑up */
            if (r < 4 && (((right >> (r + 1)) & 1) == cur_bit)) ++cnt; /* right‑down */
        }
        if (!(cnt == 0 || cnt == 2 || cnt == 3))
            return false;
    }
    return true;
}

/* Precompute transitions between pairs of columns.  The key
   is (left<<5 | cur).  for each pair, `trans` holds the set of
   possible `right` columns that keep `cur` valid.  `last_ok`
   indicates whether `cur` is valid when it has no right neighbour. */
static bool trans_ready = false;
static uint32_t trans[32][32];
static bool last_ok[32][32];

static void init_trans(void)
{
    if (trans_ready) return;
    for (uint32_t left = 0; left < 32; ++left) {
        for (uint32_t cur = 0; cur < 32; ++cur) {
            uint32_t mask = 0;
            for (uint32_t right = 0; right < 32; ++right)
                if (valid_col(left, cur, right, true, true))
                    mask |= (1u << right);
            trans[left][cur] = mask;
            last_ok[left][cur] = valid_col(left, cur, 0, true, false);
        }
    }
    trans_ready = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_trans();

    /* Special case n = 1 */
    if (n == 1) {
        seq_t cnt = 0;
        for (uint32_t c = 0; c < 32; ++c) {
            if (c & 1) continue;                    /* top‑left must be 0 */
            if (valid_col(0, c, 0, false, false))
                ++cnt;
        }
        return cnt;
    }

    const size_t STATES = 32u * 32u;
    seq_t *dp = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));
    if (!dp || !dp_next) {
        free(dp); free(dp_next);
        return (seq_t)0;
    }

    /* Initialise states for columns 1 and 2.  Column 1 must be valid
       with a (non‑existent) left neighbour; column 2 will be checked
       later by the transition logic. */
    for (uint32_t c1 = 0; c1 < 32; ++c1) {
        if (c1 & 1) continue;          /* top‑left fixed to 0 */
        for (uint32_t c2 = 0; c2 < 32; ++c2) {
            if (!valid_col(0, c1, c2, false, true))
                continue;
            dp[(c1 << 5) | c2] = (seq_t)1;
        }
    }

    /* DP over remaining columns. */
    for (idx_t step = 3; step <= n; ++step) {
        for (size_t i = 0; i < STATES; ++i)
            dp_next[i] = (seq_t)0;

        for (size_t idx = 0; idx < STATES; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;
            uint32_t left = (uint32_t)(idx >> 5);
            uint32_t cur  = (uint32_t)(idx & 31u);
            uint32_t mask = trans[left][cur];
            for (uint32_t nxt = 0; nxt < 32; ++nxt)
                if (mask & (1u << nxt))
                    dp_next[(cur << 5) | nxt] += val;
        }
        /* swap dp and dp_next */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* Sum over final states where last column has no right neighbour. */
    seq_t ans = 0;
    for (size_t idx = 0; idx < STATES; ++idx) {
        seq_t val = dp[idx];
        if (!val) continue;
        uint32_t left = (uint32_t)(idx >> 5);
        uint32_t cur  = (uint32_t)(idx & 31u);
        if (last_ok[left][cur]) ans += val;
    }

    free(dp);
    free(dp_next);
    return ans;
}
