/*
 * A251386  Number of (n+1)X(4+1) 0..2 arrays with every 2X2 subblock summing
 *          to 2 3 4 5 or 6.
 * https://oeis.org/A251386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Using dynamic programming over 5‑cell rows. Each state is a row
 *            of 5 entries from {0,1,2}. Two rows can follow each other iff
 *            for all 4 consecutive 2×2 blocks (between the rows) the sum lies
 *            in {2,3,4,5,6}. The DP iterates n times to build a (n+1)×5 array
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 3⁵ = 243 possible row states. */
#define ROWS 243

/* State table and adjacency matrix, built on first use. */
static uint8_t state_vals[ROWS][5];
static bool adjacency[ROWS][ROWS];

/* Construct the state table and adjacency matrix once. */
static void build_transition(void) {
    static bool initialised = false;
    if (initialised) return;
    initialised = true;

    /* Convert each integer id to its base‑3 digit vector. */
    for (idx_t id = 0; id < ROWS; ++id) {
        idx_t x = id;
        for (idx_t c = 0; c < 5; ++c) {
            state_vals[id][c] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* Compute allowed transitions. */
    for (idx_t s = 0; s < ROWS; ++s) {
        for (idx_t t = 0; t < ROWS; ++t) {
            bool ok = true;
            for (idx_t j = 0; j < 4; ++j) {
                idx_t sum =
                    (idx_t)state_vals[s][j]   + (idx_t)state_vals[s][j + 1] +
                    (idx_t)state_vals[t][j]   + (idx_t)state_vals[t][j + 1];
                if (sum < 2 || sum > 6) {
                    ok = false;
                    break;
                }
            }
            adjacency[s][t] = ok;
        }
    }
}

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not part of the sequence (offset 1) */

    static bool built = false;
    if (!built) {
        build_transition();
        built = true;
    }

    static seq_t dp[ROWS];
    static seq_t dp_next[ROWS];

    /* First row: every state is allowed. */
    for (idx_t i = 0; i < ROWS; ++i) {
        dp[i] = 1;
        dp_next[i] = 0;
    }

    /* Perform n transitions to reach a grid of height n+1. */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero the next‑row counter. */
        for (idx_t i = 0; i < ROWS; ++i)
            dp_next[i] = 0;

        for (idx_t s = 0; s < ROWS; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            for (idx_t t = 0; t < ROWS; ++t) {
                if (adjacency[s][t]) dp_next[t] += val;
            }
        }

        /* Copy dp_next → dp for the next iteration. */
        for (idx_t i = 0; i < ROWS; ++i)
            dp[i] = dp_next[i];
    }

    /* Sum over all possible final rows. */
    seq_t result = 0;
    for (idx_t i = 0; i < ROWS; ++i)
        result += dp[i];
    return result;
}
