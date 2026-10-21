/*
 * A297018  Number of n X 6 0..1 arrays with each 1 adjacent to 3, 4 or 6
 *          king-move neighboring 1's.
 * https://oeis.org/A297018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays in which every 1 has 3, 4 or
 *            * 6 king‑move neighboring 1's. Computed by dynamic programming
 *            over * rows. Uses a transition table for 64‑state pairs
 *            (prev,row)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Constants – use enum to guarantee compile‑time constants for C. */
enum { STATE_BITS = 6,
        STATE_CNT  = 1 << STATE_BITS,          /* 64 */
        PAIR_CNT   = STATE_CNT * STATE_CNT };  /* 4096 */

static inline bool valid_middle(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int j = 0; j < STATE_BITS; ++j) {
        if (!(curr >> j & 1U))
            continue;                          /* constraints only for 1's */
        int cnt = 0;

        /* row above */
        if (j > 0)   cnt += (prev >> (j-1)) & 1U;
        cnt += (prev >> j) & 1U;
        if (j < STATE_BITS-1) cnt += (prev >> (j+1)) & 1U;

        /* same row (exclude centre) */
        if (j > 0)   cnt += (curr >> (j-1)) & 1U;
        if (j < STATE_BITS-1) cnt += (curr >> (j+1)) & 1U;

        /* row below */
        if (j > 0)   cnt += (next >> (j-1)) & 1U;
        cnt += (next >> j) & 1U;
        if (j < STATE_BITS-1) cnt += (next >> (j+1)) & 1U;

        if (cnt != 3 && cnt != 4 && cnt != 6)
            return false;
    }
    return true;
}

/* Transition tables (one‑time pre‑computation). */
static uint8_t  next_states[PAIR_CNT][STATE_CNT];
static uint8_t  next_count [PAIR_CNT];
static unsigned char last_valid[PAIR_CNT];
static bool precomp_done = false;

/* DP scratch space. */
static seq_t dp_cur[PAIR_CNT];
static seq_t dp_next[PAIR_CNT];

seq_t a(idx_t n)
{
    /* Pre‑compute transitions once. */
    if (!precomp_done) {
        for (int prev = 0; prev < STATE_CNT; ++prev) {
            for (int curr = 0; curr < STATE_CNT; ++curr) {
                int id = prev * STATE_CNT + curr;
                uint8_t cnt = 0;
                bool end_ok = false;
                for (int next = 0; next < STATE_CNT; ++next) {
                    if (valid_middle((uint8_t)prev,
                                     (uint8_t)curr,
                                     (uint8_t)next)) {
                        next_states[id][cnt++] = (uint8_t)next;
                        if (next == 0)
                            end_ok = true;
                    }
                }
                next_count[id] = cnt;
                last_valid[id] = end_ok ? 1 : 0;
            }
        }
        precomp_done = true;
    }

    /* Initialise DP: row -1 is all zeros. */
    for (int i = 0; i < PAIR_CNT; ++i)
        dp_cur[i] = 0;
    for (int curr = 0; curr < STATE_CNT; ++curr)
        dp_cur[curr] = 1;          /* pair (0,curr) */

    seq_t *cur = dp_cur, *next = dp_next;

    /* Process rows 2 .. n. */
    for (idx_t r = 2; r <= n; ++r) {
        for (int i = 0; i < PAIR_CNT; ++i)
            next[i] = 0;

        for (int p = 0; p < PAIR_CNT; ++p) {
            seq_t cnt = cur[p];
            if (cnt == 0)
                continue;
            int curr = p & (STATE_CNT - 1);
            for (uint8_t k = 0; k < next_count[p]; ++k) {
                int nxt = next_states[p][k];
                int np  = curr * STATE_CNT + nxt;
                next[np] += cnt;
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Count valid final configurations (row n must satisfy rule with
     * an implicit zero row below). */
    seq_t result = 0;
    for (int p = 0; p < PAIR_CNT; ++p)
        if (cur[p] && last_valid[p])
            result += cur[p];

    return result;
}
