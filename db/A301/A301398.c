/*
 * A301398  Number of nX6 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A301398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A301398 computed by a transfer–matrix DP over 6‑bit rows. The
 *            sequence counts n×6 binary arrays where each cell has in
 *            {2,3,4,5,6} equal neighbors among horizontal, vertical and
 *            anti‑diagonal adjacencies; the upper‑left cell is fixed to 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Parameters of the grid. */
#define WIDTH 6
#define ROWS  64          /* 2^WIDTH possible rows */
#define SENTINEL 64       /* sentinel used for missing neighbour rows */

/* Size of the state space. */
#define STATE_COUNT ((SENTINEL + 1) * ROWS)

/* Global transition tables, built once. */
static bool valid[65][64][65];                  /* [prev][curr][next] */
static uint16_t next_state[STATE_COUNT][64];    /* list of allowed next states */
static uint8_t  next_count[STATE_COUNT];        /* length of the list */
static bool tables_built = false;

/* ------------------------------------------------------------ */
/* Helper to test if a triple of rows satisfies the adjacency
   condition for the middle row.  For each cell we count how many
   neighbours (among horizontal, vertical and anti‑diagonal
   directions) have the same value.  The count must be between 2
   and 6 (inclusive).  Sentinel rows contribute no neighbours. */
static bool triple_ok(int prev, int curr, int next)
{
    for (int j = 0; j < WIDTH; ++j) {
        int self = (curr >> j) & 1;
        int cnt  = 0;

        /* horizontal neighbours */
        if (j > 0           && ((curr >> (j-1)) & 1) == self) ++cnt;
        if (j + 1 < WIDTH   && ((curr >> (j+1)) & 1) == self) ++cnt;

        /* vertical neighbours */
        if (prev != SENTINEL && ((prev >> j) & 1)  == self) ++cnt;
        if (next != SENTINEL && ((next >> j) & 1)  == self) ++cnt;

        /* anti‑diagonal neighbours (NE‑SW direction) */
        if (prev != SENTINEL && j + 1 < WIDTH  && ((prev >> (j+1)) & 1) == self) ++cnt;
        if (next != SENTINEL && j   > 0          && ((next >> (j-1)) & 1) == self) ++cnt;

        if (cnt < 2 || cnt > 6) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Build the validity tables and the compact neighbour lists. */
static void build_tables(void)
{
    for (int prev = 0; prev <= SENTINEL; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            for (int next = 0; next <= SENTINEL; ++next) {
                valid[prev][curr][next] = triple_ok(prev, curr, next);
            }
        }
    }

    for (int prev = 0; prev <= SENTINEL; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            int id = prev * ROWS + curr;
            int k  = 0;
            for (int next = 0; next < ROWS; ++next) {
                if (valid[prev][curr][next]) {
                    next_state[id][k++] = curr * ROWS + next;
                }
            }
            next_count[id] = (uint8_t)k;
        }
    }
    tables_built = true;
}

/* ------------------------------------------------------------ */
/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (!tables_built) build_tables();

    /* DP arrays for current and next layers. */
    static seq_t dp[STATE_COUNT];
    static seq_t nxt[STATE_COUNT];
    seq_t *cur = dp, *next = nxt;

    /* Initialise to zero. */
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = 0;

    /* First row: only the leftmost bit may be zero. */
    for (int row = 0; row < ROWS; ++row) {
        if ((row & 1) == 0) {                /* upper‑left cell is 0 */
            int id = SENTINEL * ROWS + row;  /* state (none, row) */
            cur[id] = 1;
        }
    }

    /* Add subsequent rows. */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        for (int id = 0; id < STATE_COUNT; ++id) {
            seq_t val = cur[id];
            if (!val) continue;
            int cnt = next_count[id];
            for (int k = 0; k < cnt; ++k) {
                int ns = next_state[id][k];
                next[ns] += val;
            }
        }
        /* Swap buffers for next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Final validation: last row must be valid with no bottom neighbour. */
    seq_t result = 0;
    for (int id = 0; id < STATE_COUNT; ++id) {
        seq_t val = cur[id];
        if (!val) continue;
        int prev = id / ROWS;
        int curr = id % ROWS;
        if (valid[prev][curr][SENTINEL]) {
            result += val;
        }
    }
    return result;
}
