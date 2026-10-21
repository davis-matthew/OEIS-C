/*
 * A245946  Number of length n+3 0..4 arrays with some pair in every
 *          consecutive four terms totalling exactly 4.
 * https://oeis.org/A245946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   has_pair_sum4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245946: a(n) = number of length n+3 arrays over {0,1,2,3,4}
   in which every consecutive block of four terms contains at least
   one pair summing to 4.
   This implementation uses a linear‑time DP over the last three
   symbols.  The transition table is built once and reused for every
   call. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_pair_sum4.h"

#define STATES 125        /* 5^3 possible triples */
#define VALUES 5          /* alphabet size */

/* Transition table: for each state (abc) the list of next states (bcd)
   reachable by adding a symbol d such that the quadruple (a,b,c,d)
   contains a pair summing to 4.  next_count[s] gives the number of
   valid successors for state s. */
static int  next_state[STATES][VALUES];
static size_t next_count[STATES];
static bool initialized = false;

/* Build the transition table only once */
static void init_adjacency(void)
{
    for (int a = 0; a < VALUES; ++a) {
        for (int b = 0; b < VALUES; ++b) {
            for (int c = 0; c < VALUES; ++c) {
                size_t s = (size_t)a * 25 + (size_t)b * 5 + (size_t)c;
                size_t cnt = 0;
                for (int d = 0; d < VALUES; ++d) {
                    if (has_pair_sum4(a, b, c, d)) {
                        size_t ns = (size_t)b * 25 + (size_t)c * 5 + (size_t)d;
                        next_state[s][cnt++] = (int)ns;
                    }
                }
                next_count[s] = cnt;
            }
        }
    }
    initialized = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
        init_adjacency();

    /* Length of the arrays to count is n+3 */
    idx_t len = n + (idx_t)3;

    /* DP arrays holding counts for all states of the last three symbols */
    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];

    /* Initially, for length 3, every triple is a valid prefix of length 3
       (no constraint yet on a quadruple).  We treat them as having
       count 1. */
    for (size_t i = 0; i < STATES; ++i)
        dp_cur[i] = (seq_t)1;

    /* Extend the sequences one symbol at a time to reach the desired length */
    for (idx_t cur_len = 4; cur_len <= len; ++cur_len) {
        /* Zero the next vector */
        for (size_t i = 0; i < STATES; ++i)
            dp_next[i] = (seq_t)0;

        /* Transition step */
        for (size_t s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0)
                continue;
            size_t cnt = next_count[s];
            for (size_t k = 0; k < cnt; ++k) {
                size_t ns = (size_t)next_state[s][k];
                dp_next[ns] = dp_next[ns] + val;
            }
        }
        /* Move to the next length */
        for (size_t i = 0; i < STATES; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Sum all counts for the final length */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < STATES; ++i)
        result = result + dp_cur[i];

    return result;
}
