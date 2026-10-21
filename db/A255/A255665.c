/*
 * A255665  Number of length n+5 0..3 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 0..3 arrays with at most two
 *            downsteps * in every n consecutive neighbor pairs. * The
 *            sequence is computed by a DP over the 5 overlapping windows of
 *            length n * (for all n≥1). See implementation for details
 * Range:     u8 none, u16 n <= 3, u32 n <= 24, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define STATES 243 /* 3^5 possible counts (0..2) for each of the 5 windows */

static int state_counts[STATES][5];

/* Precompute the base‑3 representation for every state ID. */
static void init_state_counts(void) {
    for (int id = 0; id < STATES; ++id) {
        int x = id;
        for (int i = 0; i < 5; ++i) {
            state_counts[id][i] = x % 3;
            x /= 3;
        }
    }
}

/* Main function required by the build.  */
seq_t a(idx_t n) {
    /* The recurrence works for all n ≥ 1.  For n < 1 we return 0. */
    if (n == 0) return (seq_t)0;

    /* The number of elements in the array is n+5.               */
    /* The number of adjacent pairs (pairs we will step through) */
    /* is L-1 = n+4.  We step through these pairs one by one.   */
    const idx_t L = n + 5ULL;
    const idx_t pair_cnt = L - 1ULL;

    /* Allocate two DP layers: dp[cur/next][last_val][state]            */
    static seq_t dp[2][4][STATES];
    /* Initialize state_combinations once. */
    static bool initialized = false;
    if (!initialized) {
        init_state_counts();
        initialized = true;
    }

    /* Initialise dp for position 0 (first element).  No pairs yet.   */
    for (int v = 0; v < 4; ++v)
        for (int s = 0; s < STATES; ++s)
            dp[0][v][s] = 0;
    for (int v = 0; v < 4; ++v)
        dp[0][v][0] = 1;  /* intermediate state ID 0 = all counts zero */

    int cur = 0, nxt = 1;

    /* Iterate over every pair index p = 0 .. pair_cnt-1 (L-2).          */
    for (idx_t p = 0; p < pair_cnt; ++p) {
        /* Clear next layer. */
        for (int v = 0; v < 4; ++v)
            for (int s = 0; s < STATES; ++s)
                dp[nxt][v][s] = 0;

        /* Process all states from current layer. */
        for (int prev = 0; prev < 4; ++prev) {
            for (int sid = 0; sid < STATES; ++sid) {
                seq_t count = dp[cur][prev][sid];
                if (count == 0) continue;

                const int *old_counts = state_counts[sid];

                for (int curr = 0; curr < 4; ++curr) {
                    int down = (prev > curr) ? 1 : 0;

                    /* Copy counts to a mutable array. */
                    int cnt[5];
                    for (int i = 0; i < 5; ++i) cnt[i] = old_counts[i];

                    bool valid = true;

                    /* Update counts for windows that contain this pair. */
                    for (int s = 0; s < 5; ++s) {
                        if (s <= (int)p && (int)p <= s + (int)n - 1) {
                            if (down) {
                                ++cnt[s];
                                if (cnt[s] > 2) { valid = false; break; }
                            }
                        }
                    }
                    if (!valid) continue;

                    /* Close windows that end at this pair. */
                    for (int s = 0; s < 5; ++s) {
                        if (s + (int)n - 1 == (int)p) {
                            cnt[s] = 0;
                        }
                    }

                    /* Encode new state ID. */
                    int new_sid = 0;
                    int mult = 1;
                    for (int s = 0; s < 5; ++s) {
                        new_sid += cnt[s] * mult;
                        mult *= 3;
                    }

                    dp[nxt][curr][new_sid] += count;
                }
            }
        }
        /* Swap layers. */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* After processing all pairs, all windows are closed,
     * so only state ID 0 remains valid (all counts zero). */
    seq_t result = 0;
    for (int v = 0; v < 4; ++v)
        result += dp[cur][v][0];

    return result;
}
