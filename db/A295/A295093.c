/*
 * A295093  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 3 1s.
 * https://oeis.org/A295093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Calculated by dynamic programming over 4‑column rows using state
 *            transitions that enforce the adjacency rule. No closed form
 *            exists; the algorithm follows the definition of the sequence
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* 12 bits for a row state: 3 bits per column
   (bit0: 1 if cell is 1, bits1‑2: neighbors so far among
   left, right, up). */
#define STATES   (1U << 12)      /* 4096 */
#define MAX_CUR  (1U << 4)       /* 16 possible masks for the next row */

/* Transition tables (global, built once) */
static uint16_t *trans_flat = NULL;   /* [s * MAX_CUR + i] → next state */
static uint8_t  *trans_cnt  = NULL;   /* number of successors for state s */
static uint8_t  *final_ok   = NULL;   /* 1 if state satisfies final row rule */
static int      init_done   = 0;

/* ------------------------------------------------------------------ */
/* Helper: encode a row mask and the neighbor counts (left/right/up)
   into a 12‑bit state value.                                 */
static uint16_t encode_state(uint8_t mask, const uint8_t pc[4])
{
    uint16_t state = 0;
    for (int j = 0; j < 4; ++j)
        if (mask & (1U << j))
        {
            state |= (1U << (j * 3));                     /* cell present */
            state |= (pc[j] & 3U) << (j * 3 + 1);          /* 2 bits for count */
        }
    return state;
}

/* ------------------------------------------------------------------ */
/* Build the transition tables once – thread‑unsafe but acceptable
   in the single‑process context of the evaluation harness.         */
static void init_transitions(void)
{
    trans_flat = (uint16_t *)calloc(STATES * MAX_CUR, sizeof(uint16_t));
    trans_cnt  = (uint8_t *)calloc(STATES, sizeof(uint8_t));
    final_ok   = (uint8_t *)calloc(STATES, sizeof(uint8_t));

    for (unsigned int s = 0; s < STATES; ++s)
    {
        uint8_t prev_mask = 0;
        uint8_t pc_prev[4] = {0, 0, 0, 0};

        /* Decode the previous row from the state. */
        for (int j = 0; j < 4; ++j)
            if (s & (1U << (j * 3))) {
                prev_mask |= (1U << j);
                pc_prev[j] = (s >> (j * 3 + 1)) & 3U;
            }

        uint8_t count = 0;

        /* Consider every possible next row mask. */
        for (uint8_t cur = 0; cur < 16; ++cur)
        {
            /* Check that all 1‑cells in the previous row meet the rule
               once the vertical neighbour from cur is known. */
            int ok = 1;
            for (int j = 0; j < 4; ++j)
                if (prev_mask & (1U << j)) {
                    uint8_t finalCnt = pc_prev[j] + ((cur >> j) & 1U);
                    if (finalCnt != 1 && finalCnt != 3) { ok = 0; break; }
                }
            if (!ok) continue;

            /* Compute neighbor counts for the current row (left/right/up). */
            uint8_t pc_cur[4] = {0, 0, 0, 0};
            for (int j = 0; j < 4; ++j)
                if (cur & (1U << j)) {
                    uint8_t left  = (j > 0   && (cur >> (j - 1)) & 1U);
                    uint8_t right = (j < 3   && (cur >> (j + 1)) & 1U);
                    uint8_t up    = (prev_mask >> j) & 1U;
                    pc_cur[j] = left + right + up;          /* 0..3 */
                }

            uint16_t nxt = encode_state(cur, pc_cur);
            trans_flat[s * MAX_CUR + count++] = nxt;
        }
        trans_cnt[s] = count;
    }

    /* Final‑row validity table – no neighbour below the last row. */
    for (unsigned int s = 0; s < STATES; ++s) {
        int ok = 1;
        for (int j = 0; j < 4; ++j)
            if (s & (1U << (j * 3))) {
                uint8_t cnt = (s >> (j * 3 + 1)) & 3U;
                if (cnt != 1 && cnt != 3) { ok = 0; break; }
            }
        final_ok[s] = ok;
    }
    init_done = 1;
}

/* ------------------------------------------------------------------ */
/* Main function required by the harness.                            */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* sequence defined for n ≥ 1 */

    if (!init_done) init_transitions();

    /* Allocate DP arrays for current and next row states.          */
#ifdef __cplusplus
    seq_t *dp       = new seq_t[STATES];
    seq_t *next_dp  = new seq_t[STATES];
#else
    seq_t *dp       = (seq_t*)calloc(STATES, sizeof(seq_t));
    seq_t *next_dp  = (seq_t*)calloc(STATES, sizeof(seq_t));
#endif
    for (size_t i = 0; i < STATES; ++i) dp[i] = 0, next_dp[i] = 0;

    /* Initialisation – first row of the grid.                      */
    for (uint8_t mask = 0; mask < 16; ++mask) {
        uint8_t pc[4] = {0, 0, 0, 0};
        for (int j = 0; j < 4; ++j)
            if (mask & (1U << j)) {
                uint8_t left  = (j > 0   && (mask >> (j - 1)) & 1U);
                uint8_t right = (j < 3   && (mask >> (j + 1)) & 1U);
                pc[j] = left + right;        /* left + right, no up yet */
            }
        uint16_t state = encode_state(mask, pc);
        dp[state] += (seq_t)1;
    }

    /* Process remaining rows. Each step finalises the previous row. */
    for (idx_t row = 1; row < n; ++row) {
        /* reset next_dp */
        for (size_t i = 0; i < STATES; ++i) next_dp[i] = 0;

        for (unsigned int s = 0; s < STATES; ++s) {
            seq_t ways = dp[s];
            if (!ways) continue;
            uint8_t cnt = trans_cnt[s];
            for (uint8_t t = 0; t < cnt; ++t)
                next_dp[ trans_flat[s * MAX_CUR + t] ] += ways;
        }
        /* swap */
        seq_t *tmp = dp; dp = next_dp; next_dp = tmp;
    }

    /* Sum over final states that satisfy the rule.                 */
    seq_t result = 0;
    for (unsigned int s = 0; s < STATES; ++s)
        if (final_ok[s]) result += dp[s];

#ifdef __cplusplus
    delete [] dp; delete [] next_dp;
#else
    free(dp); free(next_dp);
#endif
    return result;
}
