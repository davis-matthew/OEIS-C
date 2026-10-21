/*
 * A246889  Number of length n+4 0..5 arrays with some pair in every
 *          consecutive five terms totalling exactly 5.
 * https://oeis.org/A246889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of length n+4 arrays over digits 0..5 such that every
 *            consecutive five terms contain at least one pair summing to 5.
 *            Implementation uses a 4‑digit automaton (state = last 4 digits)
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   split_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/split_state.h"

#define STATES 1296          /* 6^4 possible last‑4‑digit states */
#define DIGITS 6

/* Build transition table once */
static bool trans_ready = false;
static int trans[STATES][DIGITS];

static void build_transitions(void) {
    if (trans_ready) return;
    for (int s = 0; s < STATES; ++s) {
        int d3, d2, d1, d0;
        split_state(s, &d3, &d2, &d1, &d0);
        for (int e = 0; e < DIGITS; ++e) {
            /* Check if any pair among d3,d2,d1,d0,e sums to 5 */
            bool ok = false;
            if (d3 + d2 == 5 || d3 + d1 == 5 || d3 + d0 == 5 ||
                d3 + e   == 5 || d2 + d1 == 5 || d2 + d0 == 5 ||
                d2 + e   == 5 || d1 + d0 == 5 || d1 + e == 5 ||
                d0 + e   == 5) {
                ok = true;
            }
            if (ok) {
                int ns = d2 * 216 + d1 * 36 + d0 * 6 + e; /* new state */
                trans[s][e] = ns;
            } else {
                trans[s][e] = -1;
            }
        }
    }
    trans_ready = true;
}

/* DP arrays */
static seq_t cur[STATES];
static seq_t nxt[STATES];

seq_t a(idx_t n) {
    /* Sequence is defined for n >= 1 (offset 1). */
    if (n == 0) return 0; /* not part of OEIS range, but safe */

    build_transitions();

    /* Length 4 initial state: each state corresponds to exactly one
       4‑digit sequence. */
    for (idx_t s = 0; s < STATES; ++s) cur[s] = 1;

    /* Perform n iterations to reach length n+4 (starting from 4). */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero nxt */
        for (idx_t s = 0; s < STATES; ++s) nxt[s] = 0;
        /* Transition */
        for (idx_t s = 0; s < STATES; ++s) {
            seq_t v = cur[s];
            if (v == 0) continue;
            for (int e = 0; e < DIGITS; ++e) {
                int ns = trans[s][e];
                if (ns >= 0) {
                    nxt[ns] += v;
                }
            }
        }
        /* Swap cur and nxt */
        for (idx_t s = 0; s < STATES; ++s) cur[s] = nxt[s];
    }

    /* Sum all states to get total sequences of length n+4. */
    seq_t total = 0;
    for (idx_t s = 0; s < STATES; ++s) total += cur[s];
    return total;
}
