/*
 * A249843  Number of length n+4 0..7 arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 0..7 arrays with no five consecutive
 *            terms having the maximum of any two equal to the minimum of the
 *            remaining three. Computed via dynamic programming using the
 *            8⁴=4096 states (the last four terms of the array). Transitioning
 *            through the suffix of length five prevents...
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 4096
#define ALPHABET   8
#define INVALID ((unsigned short)0xFFFF)

/* Transition table: next_state[s][v] = new state index or INVALID if the
   5‑tuple formed by the previous four values in state s together with v
   violates the forbidden pattern. */
static unsigned short next_state[STATE_COUNT][ALPHABET];
static bool precomputed = false;

/* Precompute transitions once. */
static void compute_transitions(void)
{
    if (precomputed) return;
    for (int v1 = 0; v1 < ALPHABET; ++v1)
    for (int v2 = 0; v2 < ALPHABET; ++v2)
    for (int v3 = 0; v3 < ALPHABET; ++v3)
    for (int v4 = 0; v4 < ALPHABET; ++v4) {
        int state = (v1 << 9) | (v2 << 6) | (v3 << 3) | v4;
        for (int v5 = 0; v5 < ALPHABET; ++v5) {
            /* Check whether the 5‑tuple (v1..v4,v5) is allowed */
            int vals[5] = { v1, v2, v3, v4, v5 };
            bool ok = true;
            for (int i = 0; i < 5 && ok; ++i)
            for (int j = i + 1; j < 5 && ok; ++j) {
                int mx = vals[i] > vals[j] ? vals[i] : vals[j];
                int mn = 8;
                for (int k = 0; k < 5; ++k)
                    if (k != i && k != j && vals[k] < mn) mn = vals[k];
                if (mx == mn) ok = false;
            }

            if (ok) {
                int next = (v2 << 9) | (v3 << 6) | (v4 << 3) | v5;
                next_state[state][v5] = (unsigned short)next;
            } else {
                next_state[state][v5] = INVALID;
            }
        }
    }
    precomputed = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    compute_transitions();

    /* Length of the array to be counted */
    idx_t L = n + 4;   /* n=1 => L=5 */
    if (L < 5) return (seq_t)0;  /* undefined for n<1, but never needed */

    /* Two DP buffers for the current and next state counts */
    static seq_t dp[2][STATE_COUNT];
    int cur = 0, nxt = 1;

    /* Initialise for length 4: each 4‑tuple appears exactly once */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp[cur][i] = (seq_t)1;

    /* Iterate through positions 5 .. L */
    for (idx_t pos = 5; pos <= L; ++pos) {
        /* Zero the next buffer */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[nxt][i] = (seq_t)0;

        /* Transition from each state */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[cur][s];
            if (val == (seq_t)0) continue;
            unsigned short *trans = next_state[s];
            for (int v = 0; v < ALPHABET; ++v) {
                unsigned short ns = trans[v];
                if (ns != INVALID)
                    dp[nxt][ns] += val;
            }
        }

        /* Swap current and next */
        int t = cur; cur = nxt; nxt = t;
    }

    /* Sum over all final states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp[cur][i];

    return total;
}
