/*
 * A296017  Number of nX6 0..1 arrays with each 1 adjacent to 1 or 4 king-move
 *          neighboring 1s.
 * https://oeis.org/A296017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×6 0/1 arrays with each 1 adjacent to 1 or 4
 *            king‑move neighboring 1s. Computed by dynamic programming over
 *            rows
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: check the adjacency condition for a single cell row. */
static inline bool satisfies(uint64_t prev, uint64_t curr, uint64_t next) {
    for (int k = 0; k < 6; ++k) {
        if ((curr >> k) & 1ULL) {
            int cnt = 0;
            /* same row */
            if (k > 0 && ((curr >> (k-1)) & 1ULL)) ++cnt;
            if (k < 5 && ((curr >> (k+1)) & 1ULL)) ++cnt;
            /* up/down */
            if ((prev >> k) & 1ULL) ++cnt;
            if ((next >> k) & 1ULL) ++cnt;
            /* diagonals */
            if (k > 0   && ((prev >> (k-1)) & 1ULL)) ++cnt;
            if (k < 5   && ((prev >> (k+1)) & 1ULL)) ++cnt;
            if (k > 0   && ((next >> (k-1)) & 1ULL)) ++cnt;
            if (k < 5   && ((next >> (k+1)) & 1ULL)) ++cnt;
            if (cnt != 1 && cnt != 4) return false;
        }
    }
    return true;
}

enum { STATES = 64*64 };  /* (prevprev,row) combinations */

/* Pre‑computed transition masks: for each state store a 64‑bit mask whose
   bit j is set iff a new row j satisfies the condition for the middle row. */
static uint64_t trans[STATES];
static bool init_done = false;

/* Dynamic programming arrays: two buffers that we swap each iteration. */
static seq_t dp_buf[STATES * 2];
static seq_t *dp_cur = dp_buf;
static seq_t *dp_next = dp_buf + STATES;

/* Main function required by the specification. */
seq_t a(idx_t n) {
    /* Set up transition table once. */
    if (!init_done) {
        for (uint64_t pp = 0; pp < 64; ++pp) {
            for (uint64_t p = 0; p < 64; ++p) {
                uint64_t mask = 0;
                for (uint64_t next = 0; next < 64; ++next) {
                    if (satisfies(pp, p, next))
                        mask |= 1ULL << next;
                }
                trans[(pp << 6) | p] = mask;
            }
        }
        init_done = true;
    }

    /* DP initialization: before any rows we have (prevprev=0, prev=0) = 1. */
    memset(dp_cur, 0, sizeof(seq_t) * STATES);
    dp_cur[0] = 1;

    /* Process each of the n rows. */
    for (idx_t row = 0; row < n; ++row) {
        memset(dp_next, 0, sizeof(seq_t) * STATES);

        for (size_t idx = 0; idx < STATES; ++idx) {
            seq_t val = dp_cur[idx];
            if (!val) continue;
            uint64_t mask = trans[idx];
            while (mask) {
                unsigned next = __builtin_ctzll(mask);
                mask &= mask - 1;                // clear lowest set bit
                size_t new_idx = ((idx & 0x3F) << 6) | next;  /* shift state left */
                dp_next[new_idx] += val;
            }
        }
        /* swap buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Final summation: check the last row against a trailing zero row. */
    seq_t result = 0;
    for (size_t idx = 0; idx < STATES; ++idx) {
        if (satisfies((idx >> 6), (idx & 0x3F), 0)) {
            result += dp_cur[idx];
        }
    }
    return result;
}
