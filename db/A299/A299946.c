/*
 * A299946  Number of nX6 0..1 arrays with every element equal to 0, 1, 2, 4,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A299946:
   Number of n×6 binary arrays in which each element has
   a permitted number (0,1,2,4,6,7,8) of king‑move adjacent
   elements equal to itself.
   The upper‑left element is forced to 0.
   The function is memoised and works for any n ≥ 1. */
#include <stdint.h>
#include <stdbool.h>

/* Parameters of the problem */
#define COLS 6
#define ROWS_PER_BLOCK (1U << COLS)          /* 64 possible 6‑bit rows */
#define NUM_STATES (ROWS_PER_BLOCK * ROWS_PER_BLOCK) /* (prevprev,prev) pairs */
#define MAX_TRANS 64                         /* maximum successors for a state */

/* Allowed counts of equal neighbours (index = count) */
static const bool ALLOWED[9] = {
    true,  /* 0 */
    true,  /* 1 */
    true,  /* 2 */
    false, /* 3 */
    true,  /* 4 */
    false, /* 5 */
    true,  /* 6 */
    true,  /* 7 */
    true   /* 8 */
};

/* Pre‑computed tables */
static bool valid3[ROWS_PER_BLOCK][ROWS_PER_BLOCK][ROWS_PER_BLOCK];
static uint8_t trans_cnt[NUM_STATES];
static uint8_t trans_next[NUM_STATES][MAX_TRANS];
static bool init_done = false;

/* ------------------------------------------------------------------ */
/* Initialise validity tables and transition lists */
static void init(void)
{
    if (init_done) return;
    init_done = true;

    /* Compute validity of all 3‑row triples */
    for (int r0 = 0; r0 < ROWS_PER_BLOCK; ++r0) {
        for (int r1 = 0; r1 < ROWS_PER_BLOCK; ++r1) {
            for (int r2 = 0; r2 < ROWS_PER_BLOCK; ++r2) {
                bool ok = true;
                for (int c = 0; c < COLS; ++c) {
                    int v = (r1 >> c) & 1;
                    int cnt = 0;
                    /* up‑left, up, up‑right from r0 */
                    if (c > 0   && ((r0 >> (c-1)) & 1) == v) ++cnt;
                    if        (((r0 >>  c)     & 1) == v) ++cnt;
                    if (c < COLS-1 && ((r0 >> (c+1)) & 1) == v) ++cnt;
                    /* left, right in r1 */
                    if (c > 0   && ((r1 >> (c-1)) & 1) == v) ++cnt;
                    if (c < COLS-1 && ((r1 >> (c+1)) & 1) == v) ++cnt;
                    /* down‑left, down, down‑right from r2 */
                    if (c > 0   && ((r2 >> (c-1)) & 1) == v) ++cnt;
                    if        (((r2 >>  c)     & 1) == v) ++cnt;
                    if (c < COLS-1 && ((r2 >> (c+1)) & 1) == v) ++cnt;

                    if (!ALLOWED[cnt]) { ok = false; break; }
                }
                valid3[r0][r1][r2] = ok;
            }
        }
    }

    /* Build the transition lists */
    for (int idx = 0; idx < NUM_STATES; ++idx) {
        int prevprev = idx >> COLS;   /* high bits */
        int prev     = idx & (ROWS_PER_BLOCK - 1); /* low bits */
        int count = 0;
        for (int cur = 0; cur < ROWS_PER_BLOCK; ++cur) {
            if (valid3[prevprev][prev][cur]) {
                trans_next[idx][count++] = (uint8_t)cur;
            }
        }
        trans_cnt[idx] = (uint8_t)count;
    }
}

/* ------------------------------------------------------------------ */
/* Check validity of the top row (row 0) given the row underneath. */
static bool top_row_valid(uint8_t r0, uint8_t r1)
{
    for (int c = 0; c < COLS; ++c) {
        int v = (r0 >> c) & 1;
        int cnt = 0;
        /* left and right in r0 */
        if (c > 0 && ((r0 >> (c-1)) & 1) == v) ++cnt;
        if (c < COLS-1 && ((r0 >> (c+1)) & 1) == v) ++cnt;
        /* down‑left, down, down‑right from r1 */
        if (c > 0   && ((r1 >> (c-1)) & 1) == v) ++cnt;
        if        (((r1 >>  c)     & 1) == v) ++cnt;
        if (c < COLS-1 && ((r1 >> (c+1)) & 1) == v) ++cnt;

        if (!ALLOWED[cnt]) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Check validity of the bottom row (last row) given the row above. */
static bool bottom_row_valid(uint8_t rprevprev, uint8_t rprev)
{
    for (int c = 0; c < COLS; ++c) {
        int v = (rprev >> c) & 1;
        int cnt = 0;
        /* left and right in rprev */
        if (c > 0 && ((rprev >> (c-1)) & 1) == v) ++cnt;
        if (c < COLS-1 && ((rprev >> (c+1)) & 1) == v) ++cnt;
        /* up‑left, up, up‑right from rprevprev */
        if (c > 0   && ((rprevprev >> (c-1)) & 1) == v) ++cnt;
        if        (((rprevprev >>  c)     & 1) == v) ++cnt;
        if (c < COLS-1 && ((rprevprev >> (c+1)) & 1) == v) ++cnt;

        if (!ALLOWED[cnt]) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* The requested OEIS function */
seq_t a(idx_t n)
{
    /* Special case n = 1: only the top row, upper‑left fixed to 0 */
    if (n == 1) {
        return (seq_t)32;          /* 2^(6‑1) possible rows */
    }

    init();   /* build static tables once */

    /* dynamic programming arrays */
    static seq_t dp[NUM_STATES];
    static seq_t next_dp[NUM_STATES];
    for (int i = 0; i < NUM_STATES; ++i) dp[i] = 0;

    /* initialise with all valid first two rows (row0, row1) */
    for (int r0 = 0; r0 < ROWS_PER_BLOCK; ++r0) {
        if (r0 & 1) continue;          /* upper‑left cell must be 0 */
        for (int r1 = 0; r1 < ROWS_PER_BLOCK; ++r1) {
            if (top_row_valid((uint8_t)r0, (uint8_t)r1)) {
                int idx = (r0 << COLS) | r1;
                dp[idx] = 1;
            }
        }
    }

    /* process remaining rows (indices 2 .. n‑1) */
    for (idx_t row = 2; row < n; ++row) {
        for (int i = 0; i < NUM_STATES; ++i) next_dp[i] = 0;
        for (int idx = 0; idx < NUM_STATES; ++idx) {
            seq_t val = dp[idx];
            if (val == 0) continue;
            int prev = idx & (ROWS_PER_BLOCK - 1); /* previous row */
            uint8_t count = trans_cnt[idx];
            uint8_t *list = trans_next[idx];
            for (uint8_t k = 0; k < count; ++k) {
                int cur = list[k];
                int newIdx = (prev << COLS) | cur;
                next_dp[newIdx] += val;
            }
        }
        /* move next_dp into dp for the next iteration */
        for (int i = 0; i < NUM_STATES; ++i) dp[i] = next_dp[i];
    }

    /* final validation of the bottom row */
    seq_t total = 0;
    for (int idx = 0; idx < NUM_STATES; ++idx) {
        seq_t val = dp[idx];
        if (val == 0) continue;
        int prevprev = idx >> COLS;
        int prev     = idx & (ROWS_PER_BLOCK - 1);
        if (bottom_row_valid((uint8_t)prevprev, (uint8_t)prev)) {
            total += val;
        }
    }
    return total;
}
