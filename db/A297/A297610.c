/*
 * A297610  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A297610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n 0/1 arrays in which every 1 has 0, 1 or 3
 *            neighboring 1s horizontally, diagonally or antidiagonally.
 *            Computed by a transfer‑matrix DP over columns
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

extern idx_t _floor_isqrt_u64(idx_t n);   /* Provided by the build */

/* Helper tables */
static const uint8_t POP[16] = {
    0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4
};

/* Preconditions: dp tables are small, recomputed only once per program. */
static uint8_t NEIGH[16][4] = {0};
static int NEIGH_INIT = 0;

static void init_neigh(void) {
    if (NEIGH_INIT) return;
    NEIGH_INIT = 1;
    for (uint8_t pat = 0; pat < 16; ++pat) {
        for (int r = 0; r < 4; ++r) {
            uint8_t mask = 1 << r;
            if (r > 0)   mask |= 1 << (r - 1);
            if (r < 3)   mask |= 1 << (r + 1);
            NEIGH[pat][r] = mask;
        }
    }
}

/* Check a 1 at row r of pattern 'curr' against one neighboring column */
static inline int check_boundary(uint8_t curr, uint8_t neighbor) {
    for (int r = 0; r < 4; ++r) {
        if (curr & (1 << r)) {
            int c = POP[neighbor & NEIGH[curr][r]];
            if (c != 0 && c != 1 && c != 3) return 0;
        }
    }
    return 1;
}

/* Check a 1 at row r of pattern 'curr' against two neighboring columns */
static inline int check_middle(uint8_t curr, uint8_t prev, uint8_t next) {
    for (int r = 0; r < 4; ++r) {
        if (curr & (1 << r)) {
            int c = POP[prev & NEIGH[curr][r]] + POP[next & NEIGH[curr][r]];
            if (c != 0 && c != 1 && c != 3) return 0;
        }
    }
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)16;          /* All 4‑bit patterns are valid */

    init_neigh();

    /* dp[prev][curr] holds counts for the first i columns
       ending with columns pattern prev (i-1) and curr (i). */
    seq_t dp[16][16]      = {{0}};
    seq_t next_dp[16][16] = {{0}};

    /* Initial step: 2 columns.  Validate first column using only its right neighbor. */
    for (uint8_t p = 0; p < 16; ++p) {
        for (uint8_t q = 0; q < 16; ++q) {
            if (check_boundary(p, q))
                dp[p][q] = (seq_t)1;
        }
    }

    /* Process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        /* Zero next_dp */
        for (uint8_t i = 0; i < 16; ++i)
            for (uint8_t j = 0; j < 16; ++j)
                next_dp[i][j] = (seq_t)0;

        for (uint8_t p = 0; p < 16; ++p)              /* prev */
            for (uint8_t q = 0; q < 16; ++q) {        /* curr */
                seq_t val = dp[p][q];
                if (!val) continue;
                for (uint8_t r = 0; r < 16; ++r) {    /* next */
                    if (check_middle(q, p, r))
                        next_dp[q][r] += val;
                }
            }

        /* Move next_dp to dp */
        for (uint8_t i = 0; i < 16; ++i)
            for (uint8_t j = 0; j < 16; ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* Sum over all last two columns, validate the last column using only its left neighbor. */
    seq_t ans = (seq_t)0;
    for (uint8_t p = 0; p < 16; ++p)          /* column n-1 */
        for (uint8_t q = 0; q < 16; ++q) {    /* column n   */
            if (check_boundary(q, p))
                ans += dp[p][q];
        }

    return ans;
}
