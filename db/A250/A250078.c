/*
 * A250078  Number of length n+5 0..5 arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 0..5 arrays where in every *
 *            consecutive 6 terms the maximum of some two terms equals the *
 *            minimum of the remaining four. Computed via dynamic programming
 *            * over 5‑digit suffixes
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 28, u128 n <= 63,
 *            big any n
 * Helpers:   encode5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/encode5.h"

#define STATE_COUNT 7776      /* 6^5 */
#define TRANSITIONS 6        /* digits 0..5 */
#define PATTERN_COUNT 46656  /* 6^6 */

static bool          valid6[PATTERN_COUNT];
static int16_t       next_state[STATE_COUNT][TRANSITIONS];
static uint8_t       state_digits[STATE_COUNT][5];
static seq_t         state_cur[STATE_COUNT];
static seq_t         state_nxt[STATE_COUNT];
static bool          init_done = false;

/* pre‑compute the tables used by the DP */
static void init_state(void)
{
    int id, d, k, i, j, t;
    uint8_t pat[6];

    /* decode all 5‑digit states */
    for (id = 0; id < STATE_COUNT; ++id) {
        int tmp = id;
        for (k = 4; k >= 0; --k) {
            state_digits[id][k] = tmp % 6;
            tmp /= 6;
        }
    }

    /* test all 6‑digit patterns for the required property */
    for (id = 0; id < PATTERN_COUNT; ++id) {
        int tmp = id;
        for (k = 5; k >= 0; --k) {
            pat[k] = tmp % 6;
            tmp /= 6;
        }
        bool ok = false;
        for (i = 0; i < 6 && !ok; ++i) {
            for (j = i + 1; j < 6 && !ok; ++j) {
                uint8_t mx = (pat[i] > pat[j]) ? pat[i] : pat[j];
                uint8_t mn = 255;
                for (t = 0; t < 6; ++t) {
                    if (t == i || t == j) continue;
                    if (pat[t] < mn) mn = pat[t];
                }
                if (mx == mn) ok = true;
            }
        }
        valid6[id] = ok;
    }

    /* compute transitions for every state/digit */
    for (id = 0; id < STATE_COUNT; ++id) {
        for (d = 0; d < TRANSITIONS; ++d) {
            /* build 6‑digit window: current state digits + new d */
            for (k = 0; k < 5; ++k) pat[k] = state_digits[id][k];
            pat[5] = (uint8_t)d;

            /* encode 6‑digit pattern id */
            int pat_id = (((((pat[0] * 6 + pat[1]) * 6 + pat[2]) * 6 + pat[3]) * 6 + pat[4]) * 6 + pat[5]);

            if (!valid6[pat_id]) {
                next_state[id][d] = -1;      /* invalid transition */
                continue;
            }

            /* new state is last 5 digits of the window */
            int ns = state_digits[id][1];
            ns = ns * 6 + state_digits[id][2];
            ns = ns * 6 + state_digits[id][3];
            ns = ns * 6 + state_digits[id][4];
            ns = ns * 6 + d;

            next_state[id][d] = (int16_t)ns;
        }
    }
}

seq_t a(idx_t n)
{
    if (!init_done) {
        init_state();
        init_done = true;
    }

    /* initial DP state: all 5‑digit suffixes are possible with count 1 */
    for (int i = 0; i < STATE_COUNT; ++i) state_cur[i] = 1;

    seq_t *cur = state_cur;
    seq_t *nxt = state_nxt;

    for (idx_t step = 0; step < n; ++step) {
        /* clear next array */
        for (int i = 0; i < STATE_COUNT; ++i) nxt[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (int d = 0; d < TRANSITIONS; ++d) {
                int ns = next_state[s][d];
                if (ns >= 0) {
                    nxt[ns] = nxt[ns] + val;
                }
            }
        }

        /* swap pointers for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all terminal states to get the answer */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += cur[i];
    return total;
}
