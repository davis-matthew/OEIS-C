/*
 * A282526  Number of nX6 0..1 arrays with no 1 equal to more than two of its
 *          king-move neighbors.
 * https://oeis.org/A282526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with no 1 having more than
 *            two king‑move neighbors. Computed via a transfer‑matrix approach
 *            using dynamic programming over pairs of rows
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   curr_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/curr_ok.h"

/* DP function */
seq_t a(idx_t n)
{
    /* Precompute allowed next‑row bit masks for each (prev, curr) pair. */
    static uint64_t allowed_mask[64 * 64];
    static bool initialized = false;
    if (!initialized) {
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                uint64_t mask = 0ULL;
                for (int next = 0; next < 64; ++next) {
                    if (curr_ok((idx_t)prev, (idx_t)curr, (idx_t)next))
                        mask |= 1ULL << next;
                }
                allowed_mask[prev * 64 + curr] = mask;
            }
        }
        initialized = true;
    }

    /* DP state: dp[prev][curr] holds number of ways for the last two rows. */
    static seq_t dp[64][64];
    static seq_t dp_next[64][64];

    /* Initialize for the first row.  Previous virtual row is all zeros. */
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            dp[i][j] = 0;
        }
    }
    for (int r = 0; r < 64; ++r) {
        dp[0][r] = 1;
    }

    /* Add rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero dp_next */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t v = dp[prev][curr];
                if (v == 0) continue;
                uint64_t mask = allowed_mask[prev * 64 + curr];
                while (mask) {
                    unsigned next = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    dp_next[curr][next] += v;
                }
            }
        }

        /* Copy dp_next to dp for next iteration */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Sum over final states that satisfy the bottom boundary (next = 0). */
    seq_t result = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            if (curr_ok((idx_t)prev, (idx_t)curr, 0ULL)) {
                result += dp[prev][curr];
            }
        }
    }
    return result;
}
