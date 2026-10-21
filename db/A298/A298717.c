/*
 * A298717  Number of nX6 0..1 arrays with every element equal to 2, 3, 4, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0/1 arrays with upper‑left element 0 such
 *            that each cell has 2,3,4,6, or 7 equal king‑move neighbours
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   allowed_neighbour_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/allowed_neighbour_count.h"

#define NUM_COLS    6
#define ROW_BITS    6
#define ROW_VALUES  ((seq_t)1 << ROW_BITS)   /* 64 possible rows */
#define SENTINEL    64                      /* sentinel index for missing row */
#define PREV_SIZE   65                      /* 0..63 rows + sentinel */
#define CURR_SIZE   64
#define STATE_LEN   (PREV_SIZE * CURR_SIZE)

/* Transition masks: row_mask[p][c] has bit n set iff row n can follow (p,c). */
static uint64_t row_mask[PREV_SIZE][CURR_SIZE];
/* last_valid[p][c] true iff row c is valid when the next row is missing. */
static bool     last_valid[PREV_SIZE][CURR_SIZE];
static bool     tables_initialized = false;

/* Check whether cur row satisfies the rule with prev and next rows. */
static inline bool row_valid(idx_t prev, idx_t cur, idx_t next)
{
    bool prev_absent = (prev == SENTINEL);
    bool next_absent = (next == SENTINEL);

    for (int col = 0; col < NUM_COLS; ++col) {
        int val = (int)((cur >> col) & 1);
        int cnt = 0;

        /* iterate over all king‑move neighbours */
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int ncol = col + dc;
                if (ncol < 0 || ncol >= NUM_COLS) continue;

                if (dr == -1) {
                    if (prev_absent) continue;
                    cnt += ((prev >> ncol) & 1) == val;
                } else if (dr == 1) {
                    if (next_absent) continue;
                    cnt += ((next >> ncol) & 1) == val;
                } else { /* dr == 0 */
                    cnt += ((cur >> ncol) & 1) == val;
                }
            }
        }

        if (!allowed_neighbour_count(cnt))
            return false;
    }
    return true;
}

/* Build transition tables (called once). */
static void build_tables(void)
{
    if (tables_initialized) return;

    for (idx_t prev = 0; prev < PREV_SIZE; ++prev) {
        for (idx_t cur = 0; cur < CURR_SIZE; ++cur) {
            uint64_t mask = 0;
            for (idx_t nxt = 0; nxt < CURR_SIZE; ++nxt) {
                if (row_valid(prev, cur, nxt))
                    mask |= ((uint64_t)1) << nxt;
            }
            row_mask[prev][cur] = mask;
            last_valid[prev][cur] = row_valid(prev, cur, SENTINEL);
        }
    }
    tables_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1; n>=1 for the sequence */

    build_tables();

    static seq_t cur_arr[STATE_LEN];
    static seq_t nxt_arr[STATE_LEN];
    seq_t *cur = cur_arr;
    seq_t *nxt = nxt_arr;
    /* initialise to zero */
    memset(cur, 0, sizeof(cur_arr));

    /* initial state: prev row absent, first row is cur */
    for (idx_t r = 0; r < CURR_SIZE; ++r) {
        if (r & 1) continue;          /* upper‑left element must be 0 (bit 0) */
        cur[SENTINEL * CURR_SIZE + r] = 1;
    }

    /* transition for subsequent rows */
    for (idx_t step = 1; step < n; ++step) {
        memset(nxt, 0, sizeof(nxt_arr));

        for (idx_t prev = 0; prev < PREV_SIZE; ++prev) {
            for (idx_t cur_row = 0; cur_row < CURR_SIZE; ++cur_row) {
                seq_t val = cur[prev * CURR_SIZE + cur_row];
                if (!val) continue;

                uint64_t mask = row_mask[prev][cur_row];
                while (mask) {
                    int nxt_row = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    nxt[cur_row * CURR_SIZE + nxt_row] += val;
                }
            }
        }
        /* swap current and next buffers */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum over all valid ending configurations */
    seq_t ans = 0;
    for (idx_t prev = 0; prev < PREV_SIZE; ++prev) {
        for (idx_t cur_row = 0; cur_row < CURR_SIZE; ++cur_row) {
            if (last_valid[prev][cur_row]) {
                ans += cur[prev * CURR_SIZE + cur_row];
            }
        }
    }
    return ans;
}
