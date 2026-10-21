/*
 * A247721  Number of length n+3 0..3 arrays with no disjoint pairs in any
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes a(n) for the OEIS sequence A247721.  The value is
   the number of length‑(n+3) arrays over {0,1,2,3} in which, for every
   consecutive quadruple, no two disjoint pairs have the same sum.
   The code uses a 64‑state automaton and dynamic programming.  The
   offset is 1 (a(1) corresponds to n = 1).  For n ≥ 1 the DP counts
   the number of valid arrays of length n+3. */

#include <stdbool.h>
#include <stdint.h>

/* Transition tables: next_state[s][k] is the destination state, and
   next_count[s] is the number of valid transitions from state s. */
static int next_state[64][4];
static unsigned char next_count[64];
static bool transitions_ready = false;

/* Build the transition table once. */
static void build_transitions(void)
{
    if (transitions_ready) return;
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c) {
                int state = (a << 4) | (b << 2) | c;
                int cnt   = 0;
                for (int w = 0; w < 4; ++w) {
                    int s1 = a + b, s2 = c + w;
                    if (s1 == s2) continue;
                    int s3 = a + c, s4 = b + w;
                    if (s3 == s4) continue;
                    int s5 = a + w, s6 = b + c;
                    if (s5 == s6) continue;
                    int nstate = (b << 4) | (c << 2) | w;
                    next_state[state][cnt++] = nstate;
                }
                next_count[state] = cnt;
            }
    transitions_ready = true;
}

/* Main function for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1: a(1) is the first term */
    build_transitions();

    seq_t curr[64];
    seq_t next[64];

    /* All 4^3 initial triples are admissible before any condition */
    for (int i = 0; i < 64; ++i) curr[i] = 1;

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i) next[i] = 0;
        for (int i = 0; i < 64; ++i) {
            seq_t val = curr[i];
            unsigned char cnt = next_count[i];
            for (int k = 0; k < cnt; ++k) {
                int j = next_state[i][k];
                next[j] += val;
            }
        }
        for (int i = 0; i < 64; ++i) curr[i] = next[i];
    }

    seq_t result = 0;
    for (int i = 0; i < 64; ++i) result += curr[i];
    return result;
}
