/*
 * A297017  Number of nX5 0..1 arrays with each 1 adjacent to 3, 4 or 6
 *          king-move neighboring 1s.
 * https://oeis.org/A297017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP by transfer‑matrix over 3 consecutive rows. Counts the number
 *            of n×5 binary arrays where each 1 has exactly 3, 4 or 6
 *            king‑move adjacent 1s. Initial condition: row 0 (above the first
 *            real row) is all zeros
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build. */
/* ============================= Helper ============================= */

/* Check whether every 1 in the middle row has 3,4 or 6 neighboring 1s, given */
/* the previous and next rows.  Rows are represented as a 5‑bit mask.      */
static inline bool row_valid(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int j = 0; j < 5; ++j) {
        if (!(curr >> j & 1u)) continue;                  /* cell is 0, skip */
        int cnt = 0;
        /* same row neighbours */
        if (j > 0      && (curr >> (j-1) & 1u)) ++cnt;
        if (j < 4      && (curr >> (j+1) & 1u)) ++cnt;
        /* neighbours from the upper row */
        if (prev >> j & 1u)                           ++cnt;
        if (j > 0      && (prev >> (j-1) & 1u))       ++cnt;
        if (j < 4      && (prev >> (j+1) & 1u))       ++cnt;
        /* neighbours from the lower row */
        if (next >> j & 1u)                           ++cnt;
        if (j > 0      && (next >> (j-1) & 1u))       ++cnt;
        if (j < 4      && (next >> (j+1) & 1u))       ++cnt;
        /* allowed neighbour counts are 3, 4 or 6 */
        if (cnt != 3 && cnt != 4 && cnt != 6) return false;
    }
    return true;
}

/* ============================ Transitions ============================ */

#define STATE_CNT 1024               /* 32 × 32 : (prev, curr) states */
static uint8_t  trans[STATE_CNT][32];    /* destination next rows                  */
static unsigned transCnt[STATE_CNT];     /* number of valid transitions per state   */
static bool     lastValid[STATE_CNT];    /* validity with a trailing zero row       */

static void init_transitions(void)
{
    for (int a = 0; a < 32; ++a) {           /* prev row  */
        for (int b = 0; b < 32; ++b) {       /* curr row  */
            int idx = a * 32 + b;
            unsigned cnt = 0;
            for (int r = 0; r < 32; ++r) {   /* candidate next row */
                if (row_valid((uint8_t)a, (uint8_t)b, (uint8_t)r))
                    trans[idx][cnt++] = (uint8_t)r;
            }
            transCnt[idx] = cnt;
            lastValid[idx] = row_valid((uint8_t)a, (uint8_t)b, 0);
        }
    }
}

/* ============================= Main ============================= */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used, but safety */

    /* Static state for incremental DP */
    static bool init_done = false;
    static seq_t *dp_cur = NULL;
    static seq_t *dp_nxt = NULL;
    static idx_t cur_n = 0;               /* number of rows processed so far */

    if (!init_done) {
        dp_cur = (seq_t*)malloc(sizeof(seq_t) * STATE_CNT);
        dp_nxt = (seq_t*)malloc(sizeof(seq_t) * STATE_CNT);
        if (!dp_cur || !dp_nxt) return 0; /* allocation failure – graceful exit */

        /* initialise for n = 1 : prev row = all zeros, curr row = any pattern */
        for (int i = 0; i < STATE_CNT; ++i) dp_cur[i] = 0;
        for (int a = 0; a < 32; ++a) dp_cur[a] = 1;   /* state (0,a) */
        cur_n = 1;
        init_transitions();
        init_done = true;
    }

    /* Extend DP up to the required row count */
    while (cur_n < n) {
        for (int i = 0; i < STATE_CNT; ++i) dp_nxt[i] = 0;

        for (int idx = 0; idx < STATE_CNT; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            int a = idx / 32;
            int b = idx % 32;
            unsigned tc = transCnt[idx];
            for (unsigned t = 0; t < tc; ++t) {
                int r = trans[idx][t];
                int newIdx = b * 32 + r;
                dp_nxt[newIdx] += val;
            }
        }

        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
        ++cur_n;
    }

    /* Final answer: add a trailing zero row and sum all valid end states */
    seq_t ans = 0;
    for (int idx = 0; idx < STATE_CNT; ++idx) {
        if (dp_cur[idx] == 0) continue;
        if (lastValid[idx]) ans += dp_cur[idx];
    }
    return ans;
}
