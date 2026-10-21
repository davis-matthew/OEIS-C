/*
 * A295211  Number of nX6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 2 or 3 1s.
 * https://oeis.org/A295211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Count n×6 0/1 arrays in which every 1 has exactly 2 or 3
 *  orthogonal neighbours (horizontally or vertically). */

#include <stdint.h>
#include <stdbool.h>

#define COLS 6          /* fixed number of columns */

/* Pre‑computed data – initialised once on first call */
static unsigned int horiz_deg[64][COLS];  /* horizontal neighbour count per bit */
static uint64_t trans[64][64];            /* bitset of allowed next rows */
static bool     last_valid[64][64];       /* is the second row usable as final? */
static bool     first_ok[64];             /* first row patterns that are possible */
static bool     init_done = false;

/* initialise tables */
static void init(void)
{
    /* horizontal adjacency per mask */
    for (int mask = 0; mask < (1 << COLS); ++mask) {
        for (int j = 0; j < COLS; ++j) {
            unsigned int left  = (j > 0      && (mask & (1 << (j - 1)))) ? 1 : 0;
            unsigned int right = (j < COLS-1 && (mask & (1 << (j + 1)))) ? 1 : 0;
            horiz_deg[mask][j] = left + right;
        }
    }

    /* tables for transitions and final‑row validity */
    for (int a = 0; a < (1 << COLS); ++a) {
        for (int b = 0; b < (1 << COLS); ++b) {
            uint64_t allow = 0;
            for (int c = 0; c < (1 << COLS); ++c) {
                bool ok = true;
                /* check every 1 in row b with up from a and down from c */
                for (int j = 0; j < COLS; ++j) {
                    if (b & (1 << j)) {
                        int up   = (a >> j) & 1;
                        int horiz = horiz_deg[b][j];
                        int base  = up + horiz;
                        int down  = (c >> j) & 1;
                        int total = base + down;
                        if (total != 2 && total != 3) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok) allow |= (1ULL << c);
            }
            trans[a][b] = allow;

            /* final‑row validity (no down neighbour) */
            bool last_ok = true;
            for (int j = 0; j < COLS; ++j) {
                if (b & (1 << j)) {
                    int up   = (a >> j) & 1;
                    int horiz = horiz_deg[b][j];
                    int base  = up + horiz;
                    if (base != 2 && base != 3) {
                        last_ok = false;
                        break;
                    }
                }
            }
            last_valid[a][b] = last_ok;
        }
    }

    /* determine which first‑row patterns are possible (no up neighbour) */
    for (int b = 0; b < (1 << COLS); ++b) {
        bool ok = true;
        for (int j = 0; j < COLS; ++j) {
            if (b & (1 << j)) {
                int horiz = horiz_deg[b][j];
                if (horiz == 0) {        /* base would be 0 – impossible */
                    ok = false;
                    break;
                }
            }
        }
        first_ok[b] = ok;
    }

    init_done = true;
}

/* DP arrays: dp[prev][cur] – number of ways up to current row cur
 * with previous row prev */
static seq_t dp[1 << COLS][1 << COLS];
static seq_t next_dp[1 << COLS][1 << COLS];

seq_t a(idx_t n)
{
    if (!init_done) init();

    /* reset dp */
    for (int i = 0; i < (1 << COLS); ++i)
        for (int j = 0; j < (1 << COLS); ++j)
            dp[i][j] = 0;

    /* first row: prev = 0 */
    for (int b = 0; b < (1 << COLS); ++b)
        if (first_ok[b])
            dp[0][b] = 1;

    /* process subsequent rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < (1 << COLS); ++i)
            for (int j = 0; j < (1 << COLS); ++j)
                next_dp[i][j] = 0;

        for (int a = 0; a < (1 << COLS); ++a) {
            for (int b = 0; b < (1 << COLS); ++b) {
                seq_t ways = dp[a][b];
                if (!ways) continue;
                uint64_t mask = trans[a][b];
                while (mask) {
                    int c = __builtin_ctzll(mask);
                    next_dp[b][c] += ways;
                    mask &= mask - 1;
                }
            }
        }

        /* swap dp and next_dp */
        for (int i = 0; i < (1 << COLS); ++i)
            for (int j = 0; j < (1 << COLS); ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* sum over final rows that satisfy the ending condition */
    seq_t ans = 0;
    for (int a = 0; a < (1 << COLS); ++a)
        for (int b = 0; b < (1 << COLS); ++b)
            if (last_valid[a][b])
                ans += dp[a][b];

    return ans;
}
