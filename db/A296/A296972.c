/*
 * A296972  Number of nX6 0..1 arrays with each 1 adjacent to 2, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses a DP over row pairs for n × 6 binary arrays with the
   adjacency constraint that each 1 must have 2, 4 or 5 neighboring 1s
   (king‑move adjacency).  The routine counts all such arrays of height n. */

#include <stdbool.h>
#include <stdint.h>

/* Constants for the 6‑column grid. */
static const int COLS = 6;
static const uint64_t ROW_MASK = 0x3F;          /* 6 bits set */

/* Precomputed transition masks: allowed_next[prev][curr] is a 64‑bit
   mask where bit k is 1 iff a row 'next' = k is valid when the
   previous row is 'prev' and the current row is 'curr'. */
static uint64_t allowed_next[64][64];
static bool transitions_precomputed = false;

/* Check whether every 1 in 'curr' has a neighbour count in {2,4,5}
   given the previous row 'prev' and the next row 'next'. */
static bool curr_valid(uint8_t prev, uint8_t curr, uint8_t next) {
    for (int j = 0; j < COLS; ++j) {
        if (!(curr & (1U << j)))
            continue;                /* only examine cells that contain 1 */

        int cnt = 0;

        /* Neighbours in the previous row (vertical + diagonal). */
        if (prev & (1U << j))
            ++cnt;
        if (j > 0 && prev & (1U << (j - 1)))
            ++cnt;
        if (j < COLS - 1 && prev & (1U << (j + 1)))
            ++cnt;

        /* Horizontal neighbours in the current row. */
        if (j > 0 && curr & (1U << (j - 1)))
            ++cnt;
        if (j < COLS - 1 && curr & (1U << (j + 1)))
            ++cnt;

        /* Neighbours in the next row (vertical + diagonal). */
        if (next & (1U << j))
            ++cnt;
        if (j > 0 && next & (1U << (j - 1)))
            ++cnt;
        if (j < COLS - 1 && next & (1U << (j + 1)))
            ++cnt;

        if (cnt != 2 && cnt != 4 && cnt != 5)
            return false;
    }
    return true;
}

/* Build the transition table once. */
static void precompute_transitions(void) {
    if (transitions_precomputed)
        return;
    for (uint8_t prev = 0; prev < 64; ++prev) {
        for (uint8_t curr = 0; curr < 64; ++curr) {
            uint64_t mask = 0;
            for (uint8_t next = 0; next < 64; ++next)
                if (curr_valid(prev, curr, next))
                    mask |= (1ULL << next);
            allowed_next[prev][curr] = mask;
        }
    }
    transitions_precomputed = true;
}

/* OEIS A296972 : number of n x 6 0/1 arrays with the adjacency rule. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;                 /* not defined below offset */

    precompute_transitions();

    static seq_t dp[64][64];
    static seq_t dp_next[64][64];

    /* Reset dp to zero. */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = (seq_t)0;

    /* First row: previous row is all zeros. */
    for (int row0 = 0; row0 < 64; ++row0)
        dp[0][row0] = (seq_t)1;

    /* Process remaining rows. */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == (seq_t)0)
                    continue;
                uint64_t mask = allowed_next[prev][curr];
                for (int next = 0; next < 64; ++next) {
                    if (mask & (1ULL << next))
                        dp_next[curr][next] += ways;
                }
            }
        }

        /* Copy dp_next into dp for the next iteration. */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Final check: last row must be valid with next row all zeros. */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            seq_t ways = dp[prev][curr];
            if (ways == (seq_t)0)
                continue;
            if (allowed_next[prev][curr] & (1ULL << 0))
                ans += ways;
        }
    }
    return ans;
}
