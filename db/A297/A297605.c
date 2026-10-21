/*
 * A297605  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A297605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297605: counts n×6 binary arrays where each 1 has 0, 1, or 3
   neighboring 1s among the 6 adjacent cells (horizontal, diagonal, or
   antidiagonal).  Computed by a transfer‑matrix dynamic programming over
   rows. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Checks whether a given 6‑bit row satisfies the rule when the upper
   row is 'up' and the lower row is 'down'.  Only horizontal, diagonal
   and antidiagonal neighbours are considered; vertical neighbours are
   ignored. */
static bool check_row(uint8_t row, uint8_t up, uint8_t down)
{
    for (int j = 0; j < 6; ++j) {
        if ((row >> j) & 1u) {
            int cnt = 0;
            /* horizontal neighbours */
            if (j > 0 && (row >> (j - 1)) & 1u) ++cnt;
            if (j < 5 && (row >> (j + 1)) & 1u) ++cnt;
            /* diagonal neighbours from the row above */
            if (j > 0 && (up >> (j - 1)) & 1u) ++cnt;
            if (j < 5 && (up >> (j + 1)) & 1u) ++cnt;
            /* diagonal neighbours from the row below */
            if (j > 0 && (down >> (j - 1)) & 1u) ++cnt;
            if (j < 5 && (down >> (j + 1)) & 1u) ++cnt;
            if (cnt != 0 && cnt != 1 && cnt != 3) return false;
        }
    }
    return true;
}

/* Precomputation tables for the transfer matrix.  A state is a pair
   (prev, curr) of 6‑bit rows.  For each state we store all admissible
   next rows such that curr satisfies the rule with neighbours prev
   and next. */
static bool   init_done = false;
static bool   valid_start[64][64];
static unsigned char next_count[4096];
static unsigned char next_list[4096 * 64];

static void precompute(void)
{
    /* Determine which pairs (row1,row2) are valid as the first two rows. */
    for (uint8_t r1 = 0; r1 < 64; ++r1)
        for (uint8_t r2 = 0; r2 < 64; ++r2)
            valid_start[r1][r2] = check_row(r1, 0u, r2);

    /* Build transition lists. */
    for (uint8_t prev = 0; prev < 64; ++prev) {
        for (uint8_t curr = 0; curr < 64; ++curr) {
            int idx = (prev << 6) + curr;
            unsigned char pos = 0;
            for (uint8_t next = 0; next < 64; ++next) {
                if (check_row(curr, prev, next)) {
                    next_list[idx * 64 + pos++] = next;
                }
            }
            next_count[idx] = pos;
        }
    }
    init_done = true;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (!init_done) precompute();

    /* n == 1: count rows with all neighbours zero. */
    if (n == 1) {
        seq_t res = 0;
        for (uint8_t r = 0; r < 64; ++r)
            if (check_row(r, 0u, 0u))
                res += (seq_t)1;
        return res;
    }

    static seq_t dp[4096];
    static seq_t dp_next[4096];

    /* Reset dp arrays for this call. */
    for (int i = 0; i < 4096; ++i) dp[i] = 0;

    /* Initialise with all valid first two rows. */
    for (uint8_t r1 = 0; r1 < 64; ++r1) {
        for (uint8_t r2 = 0; r2 < 64; ++r2) {
            if (valid_start[r1][r2]) {
                int idx = (r1 << 6) | r2;
                dp[idx] = (seq_t)1;
            }
        }
    }

    /* Process rows 3 … n.  After each iteration dp holds counts for the
       last two rows processed. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 4096; ++i) dp_next[i] = 0;

        for (int s = 0; s < 4096; ++s) {
            seq_t cnt = dp[s];
            if (cnt == (seq_t)0) continue;
            unsigned char numSucc = next_count[s];
            const unsigned char *succ = next_list + s * 64;
            uint8_t curr = s & 63;
            for (unsigned char k = 0; k < numSucc; ++k) {
                uint8_t nextRow = succ[k];
                int ns = ((curr << 6) | nextRow);
                dp_next[ns] += cnt;
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < 4096; ++i) dp[i] = dp_next[i];
    }

    /* Final check for the last row (row n), whose lower neighbour is zero. */
    seq_t ans = 0;
    for (int s = 0; s < 4096; ++s) {
        seq_t cnt = dp[s];
        if (cnt == (seq_t)0) continue;
        uint8_t prev = s >> 6;
        uint8_t curr = s & 63;
        if (check_row(curr, prev, 0u))
            ans += cnt;
    }
    return ans;
}
