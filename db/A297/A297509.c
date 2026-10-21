/*
 * A297509  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n 0–1 matrices such that every 1 has exactly
 *            1 or 2 of its horizontally or diagonally adjacent 1‑neighbours.
 *            (Adjacency is horizontal and the two diagonals in the
 *            neighbouring columns.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   has_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a transfer matrix over bit patterns of
   two successive columns.  The transition matrix is pre‑computed
   once.  Complexity: O(n·16²·16), i.e. about 4 000 operations per
   step, well below the time limit for n ≤ 10 000.  */
/* The code is valid C11 and C++17.  */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/has_one.h"

/* check that every 1 in column cur has 1 or 2 neighbours given
   left column prev and right column next (both 4‑bit patterns) */
static bool valid_transition(uint8_t prev, uint8_t cur, uint8_t next) {
    for (unsigned r = 0; r < 4; ++r) {
        if (!has_one(cur, r)) continue;
        unsigned l = 0, rgt = 0;
        /* left neighbours from prev: same row and diagonals */
        if (has_one(prev, r)) ++l;
        if (r > 0      && has_one(prev, r-1)) ++l;
        if (r + 1 < 4  && has_one(prev, r+1)) ++l;
        /* right neighbours from next: same row and diagonals */
        if (has_one(next, r)) ++rgt;
        if (r > 0      && has_one(next, r-1)) ++rgt;
        if (r + 1 < 4  && has_one(next, r+1)) ++rgt;
        unsigned total = l + rgt;
        if (total != 1 && total != 2) return false;
    }
    return true;
}

static uint16_t transition_mask[16][16];
static bool initialized = false;

/* Build the transition mask once.  transition_mask[prev][cur]
   is a 16‑bit mask whose bit 'next' is set iff cur is allowed
   between prev and next. */
static void init_transition_mask(void) {
    if (initialized) return;
    for (uint8_t prev = 0; prev < 16; ++prev) {
        for (uint8_t cur = 0; cur < 16; ++cur) {
            uint16_t mask = 0;
            for (uint8_t next = 0; next < 16; ++next) {
                if (valid_transition(prev, cur, next))
                    mask |= (1 << next);
            }
            transition_mask[prev][cur] = mask;
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    /* dp[prev][cur] = number of ways to build the processed
       columns, currently the last two columns are 'prev' and 'cur'.
       'prev' is the column at position i-1, 'cur' at position i.   */
    static seq_t dp[16][16], ndp[16][16];
    init_transition_mask();

    /* initialise with the first (leftmost) column.  Dummy left
       column is 0. */
    memset(dp, 0, sizeof dp);
    for (uint8_t cur = 0; cur < 16; ++cur)
        dp[0][cur] = (seq_t)1;

    /* process columns 2..n */
    for (idx_t step = 1; step < n; ++step) {
        memset(ndp, 0, sizeof ndp);
        for (uint8_t prev = 0; prev < 16; ++prev) {
            for (uint8_t cur = 0; cur < 16; ++cur) {
                seq_t val = dp[prev][cur];
                if (!val) continue;
                uint16_t mask = transition_mask[prev][cur];
                /* iterate over allowed next columns */
                for (uint8_t next = 0; next < 16; ++next) {
                    if (mask & (1 << next))
                        ndp[cur][next] += val;
                }
            }
        }
        /* copy ndp back to dp for next step */
        memcpy(dp, ndp, sizeof dp);
    }

    /* final step: require that the rightmost column 'cur'
       satisfies the adjacency constraint with dummy right column 0. */
    seq_t ans = 0;
    for (uint8_t prev = 0; prev < 16; ++prev) {
        for (uint8_t cur = 0; cur < 16; ++cur) {
            if (dp[prev][cur] && valid_transition(prev, cur, 0))
                ans += dp[prev][cur];
        }
    }

    return ans;
}
