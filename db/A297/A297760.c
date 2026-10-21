/*
 * A297760  Number of n X 6 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 2, 3 or 4 neighboring 1's.
 * https://oeis.org/A297760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of n×6 0..1 arrays in which every 1 has between 2 and
   4 neighbors among horizontal, diagonal up-left/up-right and
   diagonal down-left/down-right positions.  A dynamic‑programming routine
   over consecutive row pairs is used. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_ROWS 64
#define STATE_CNT (MAX_ROWS * MAX_ROWS)   /* 4096 */

/* Check whether the middle row satisfies the adjacency rule, given the
   previous and next rows.  Vertical adjacency is not counted. */
static bool valid_transition(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int j = 0; j < 6; ++j) {
        if (!(curr & (1u << j))) continue;      /* cell is 0 */
        int cnt = 0;
        /* diagonal up-left, up-right */
        if (j > 0 && (prev & (1u << (j - 1)))) ++cnt;
        if (j < 5 && (prev & (1u << (j + 1)))) ++cnt;
        /* horizontal left/right */
        if (j > 0 && (curr & (1u << (j - 1)))) ++cnt;
        if (j < 5 && (curr & (1u << (j + 1)))) ++cnt;
        /* diagonal down-left, down-right */
        if (j > 0 && (next & (1u << (j - 1)))) ++cnt;
        if (j < 5 && (next & (1u << (j + 1)))) ++cnt;
        if (cnt < 2 || cnt > 4) return false;
    }
    return true;
}

/* Transition tables.  nexts[state][k] gives the k‑th admissible next row
   for the state (prev, curr).  nextCnt[state] counts the admissible
   next rows.  finalValid[state] tells whether the curr row is valid
   when the following row is 0. */
static uint8_t   nexts[STATE_CNT][MAX_ROWS];
static uint8_t   nextCnt[STATE_CNT];
static bool     finalValid[STATE_CNT];
static bool     precomp_done = false;

static void init_transitions(void)
{
    if (precomp_done) return;
    precomp_done = true;
    for (uint8_t prev = 0; prev < MAX_ROWS; ++prev) {
        for (uint8_t curr = 0; curr < MAX_ROWS; ++curr) {
            uint16_t idx = (prev << 6) | curr;
            uint8_t c = 0;
            for (uint8_t nxt = 0; nxt < MAX_ROWS; ++nxt) {
                if (valid_transition(prev, curr, nxt))
                    nexts[idx][c++] = nxt;
            }
            nextCnt[idx] = c;
            finalValid[idx] = valid_transition(prev, curr, 0);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    init_transitions();

    seq_t dp_curr[STATE_CNT];
    seq_t dp_next[STATE_CNT];

    /* zero all dp arrays */
    for (size_t i = 0; i < STATE_CNT; ++i) {
        dp_curr[i] = (seq_t)0;
        dp_next[i] = (seq_t)0;
    }

    /* first row: previous row is all zeros */
    for (uint8_t curr = 0; curr < MAX_ROWS; ++curr) {
        dp_curr[curr] = (seq_t)1;   /* state index (0<<6)|curr == curr */
    }

    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (size_t idx = 0; idx < STATE_CNT; ++idx) {
            if (dp_curr[idx] && finalValid[idx])
                ans += dp_curr[idx];
        }
        return ans;
    }

    /* Process the remaining rows */
    for (idx_t step = 0; step <= n - 2; ++step) {
        /* zero dp_next */
        for (size_t i = 0; i < STATE_CNT; ++i)
            dp_next[i] = (seq_t)0;

        for (size_t idx = 0; idx < STATE_CNT; ++idx) {
            seq_t val = dp_curr[idx];
            if (!val) continue;
            uint8_t *arr = nexts[idx];
            uint8_t  c   = nextCnt[idx];
            uint8_t  curr = (uint8_t)(idx & 0x3F);
            for (uint8_t k = 0; k < c; ++k) {
                uint8_t nxt = arr[k];
                size_t nextIdx = ((size_t)curr << 6) | nxt;
                dp_next[nextIdx] += val;
            }
        }

        /* move dp_next to dp_curr */
        for (size_t i = 0; i < STATE_CNT; ++i)
            dp_curr[i] = dp_next[i];
    }

    /* Final row validation */
    seq_t ans = (seq_t)0;
    for (size_t idx = 0; idx < STATE_CNT; ++idx) {
        if (dp_curr[idx] && finalValid[idx])
            ans += dp_curr[idx];
    }
    return ans;
}
