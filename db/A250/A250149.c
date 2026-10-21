/*
 * A250149  Number of length n+6 0..3 arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 13, u64 n <= 33, u128 n <= 74,
 *            big any n
 * Helpers:   window_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes
   a(n) = number of length n+6 0..3 arrays such that
   every 7 consecutive terms contain a pair of terms whose
   maximum equals the minimum of the other five terms.
   Computation is performed by dynamic programming over a
   deterministic finite automaton with 4⁶ = 4096 states
   representing the last six digits of the array.
   For each n (n ≥ 1) the algorithm runs in O(n·4096·4)
   time and O(4096) memory. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/window_valid.h"

#define STATE_COUNT 4096
#define MASK        0xFFF     /* lower 12 bits (6 digits in base 4) */

/* Transition table: next_state[s][d] = state after appending digit d,
   or -1 if the resulting 7‑digit window does not satisfy the condition.
   int16_t is enough because STATE_COUNT ≤ 4095 < 32768. */
static int16_t next_state[STATE_COUNT][4];
static bool    transitions_ready = false;

/* Build the transition table once, lazily. */
static void init_transitions(void) {
    if (transitions_ready) return;
    uint8_t state_digits[6];
    uint8_t w[7];

    for (int s = 0; s < STATE_COUNT; ++s) {
        int temp = s;
        /* Decode state into six base‑4 digits: a0 (most significant) … a5 */
        for (int pos = 5; pos >= 0; --pos) {
            state_digits[pos] = temp & 3;
            temp >>= 2;
        }
        for (int d = 0; d < 4; ++d) {
            /* Build the 7‑digit window */
            for (int pos = 0; pos < 6; ++pos) w[pos] = state_digits[pos];
            w[6] = (uint8_t)d;
            if (window_valid(w)) {
                int ns = ((s << 2) & MASK) | d; /* drop most significant digit */
                next_state[s][d] = (int16_t)ns;  /* ns ∈ [0,4095] */
            } else {
                next_state[s][d] = -1;
            }
        }
    }
    transitions_ready = true;
}

/* DP arrays for the automaton states. */
static seq_t dp[STATE_COUNT];
static seq_t ndp[STATE_COUNT];

seq_t a(idx_t n)
{
    /* n is the index in the OEIS sequence, corresponding to
       an array of length n+6.  We start from length 6 and
       add n digits. */

    idx_t steps = n;           /* number of digits to add */
    if (steps == 0) return 0;  /* non‑existent case; minimal index is 1 */

    init_transitions();

    seq_t *cur  = dp;
    seq_t *next = ndp;

    /* Init counts for all 6‑digit prefixes: each of the 4⁶ sequences
       occurs once as a starting state. */
    for (int s = 0; s < STATE_COUNT; ++s) cur[s] = 1;

    for (idx_t step = 0; step < steps; ++step) {
        /* Zero next array */
        for (int s = 0; s < STATE_COUNT; ++s) next[s] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (int d = 0; d < 4; ++d) {
                int16_t ns = next_state[s][d];
                if (ns < 0) continue;
                next[ns] = next[ns] + val;
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum all counts to obtain the total number of valid arrays. */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        result = result + cur[s];
    }
    return result;
}
