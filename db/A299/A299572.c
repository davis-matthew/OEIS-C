/*
 * A299572  Number of nX6 0..1 arrays with every element equal to 1, 2, 4, 5,
 *          7 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays where every cell has *
 *            1,2,4,5,7 or 8 same-valued king‑move neighbors, * and the
 *            upper‑left cell is 0. The sequence is computed * by a
 *            transfer‑matrix DP with state = pair of successive rows
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   row_id_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>   /* for memset */
#include "../utils/row_id_valid.h"

/* Dimensions for a 6×n grid */
#define ROWS 6
#define ROWS_BITS 6
#define MAX_ROW 64          /* 2^6 possible rows */

/* Transition table: for each (prev,cur) pair gives list of nxt rows
 * that satisfy the constraint for the 'cur' row. */
static unsigned char nxt_table[4096][64]   /* [prev*64+cur][k] = nxt */
    __attribute__ ((aligned (64)));
static unsigned char nxt_cnt[4096]         /* number of valid nxts per pair */
    __attribute__ ((aligned (64)));
static int init_done = 0;

/* DP arrays for counts */
static seq_t dp[4096];
static seq_t ndp[4096];

seq_t a(idx_t n)
{
    /* initialise transition table once */
    if (!init_done) {
        for (int prev = 0; prev < MAX_ROW; ++prev) {
            for (int cur = 0; cur < MAX_ROW; ++cur) {
                int idx = prev * MAX_ROW + cur;
                int k = 0;
                for (int nxt = 0; nxt < MAX_ROW; ++nxt) {
                    if (row_id_valid(prev, cur, nxt, 1, 1))
                        nxt_table[idx][k++] = (unsigned char)nxt;
                }
                nxt_cnt[idx] = (unsigned char)k;
            }
        }
        init_done = 1;
    }

    /* Special case n == 1 */
    if (n == 1) {
        seq_t sum = (seq_t)0;
        for (int row = 0; row < MAX_ROW; ++row) {
            if ((row & 1) != 0) continue;        /* upper‑left must be 0 */
            if (row_id_valid(0, row, 0, 0, 0))
                ++sum;
        }
        return sum;
    }

    /* Initialise dp with all valid (row0, row1) pairs */
    memset(dp, 0, sizeof(dp));
    for (int row0 = 0; row0 < MAX_ROW; ++row0) {
        if ((row0 & 1) != 0) continue; /* upper‑left 0 */
        for (int row1 = 0; row1 < MAX_ROW; ++row1) {
            if (row_id_valid(row0, row0, row1, 0, 1)) {
                int idx = row0 * MAX_ROW + row1;
                dp[idx] = (seq_t)1;
            }
        }
    }

    /* Transition through remaining rows */
    idx_t steps = n - 2;  /* number of additional rows beyond the first two */
    for (idx_t s = 0; s < steps; ++s) {
        memset(ndp, 0, sizeof(ndp));
        for (int idx = 0; idx < 4096; ++idx) {
            seq_t cur_count = dp[idx];
            if (cur_count == (seq_t)0) continue;

            int cur_row = idx % MAX_ROW;
            int k = nxt_cnt[idx];
            unsigned char *list = nxt_table[idx];
            for (int t = 0; t < k; ++t) {
                int nxt_row = list[t];
                int new_idx = cur_row * MAX_ROW + nxt_row;
                ndp[new_idx] += cur_count;
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Sum over final states where the last row satisfies constraints */
    seq_t result = (seq_t)0;
    for (int idx = 0; idx < 4096; ++idx) {
        seq_t cur_count = dp[idx];
        if (cur_count == (seq_t)0) continue;

        int prev_row = idx / MAX_ROW;
        int cur_row = idx % MAX_ROW;

        if (row_id_valid(prev_row, cur_row, 0, 1, 0))
            result += cur_count;
    }

    return result;
}
