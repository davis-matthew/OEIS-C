/*
 * A249228  Number of length n+5 0..3 arrays with no six consecutive terms
 *          having two times the sum of any two elements equal to the sum of
 *          the remaining four.
 * https://oeis.org/A249228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of length n+5 arrays over {0,1,2,3} with no six
 *            consecutive terms such that 2*(sum of any two) equals the sum of
 *            the remaining four. The program counts these sequences via a DFA
 *            of order 5, using a pre‑computed lookup of forbidden 6‑tuples
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 35, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* State space: 4^5 = 1024 */
#define STATE_COUNT 1024
#define ALPHABET   4
#define STEP_COUNT 5 /* size of state */

static seq_t dp[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];
static bool transition_ok[STATE_COUNT][ALPHABET];
static bool transitions_inited = false;

/* Pre‑compute forbidden 6‑tuples and transitions */
static void init_transition_table(void) {
    if (transitions_inited) return;

    bool forbidden[4096];
    for (int idx = 0; idx < 4096; ++idx) {
        int digs[6];
        int tmp = idx;
        for (int i = 5; i >= 0; --i) {
            digs[i] = tmp % 4;
            tmp /= 4;
        }
        int sum = 0;
        for (int i = 0; i < 6; ++i) sum += digs[i];
        bool bad = false;
        for (int i = 0; i < 6 && !bad; ++i)
            for (int j = i + 1; j < 6; ++j)
                if (3 * (digs[i] + digs[j]) == sum) {
                    bad = true;
                    break;
                }
        forbidden[idx] = bad;
    }

    for (int s = 0; s < STATE_COUNT; ++s)
        for (int v = 0; v < ALPHABET; ++v) {
            int idx6 = s * 4 + v;          /* encode 6‑tuple */
            transition_ok[s][v] = !forbidden[idx6];
        }

    transitions_inited = true;
}

seq_t a(idx_t n) {
    /* n >= 1; sequence length = n+5; we need n transitions */
    init_transition_table();

    /* Initially any 5‑tuple is possible */
    for (int s = 0; s < STATE_COUNT; ++s)
        dp[s] = (seq_t)1;

    /* Perform n transitions */
    for (idx_t step = 0; step < n; ++step) {
        for (int s = 0; s < STATE_COUNT; ++s)
            dp_next[s] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            for (int v = 0; v < ALPHABET; ++v) {
                if (!transition_ok[s][v]) continue;
                /* new state drops the first symbol of s */
                int new_state = (s % 256) * 4 + v; /* 4^4 = 256 */
                dp_next[new_state] += val;
            }
        }

        /* swap dp and dp_next */
        for (int s = 0; s < STATE_COUNT; ++s)
            dp[s] = dp_next[s];
    }

    /* Sum all ending states to obtain total sequences */
    seq_t total = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        total += dp[s];
    return total;
}
