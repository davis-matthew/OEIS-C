/*
 * A248999  Number of length n+4 0..6 arrays with no five consecutive terms
 *          having two times the sum of any three elements equal to three
 *          times the sum of the remaining two.
 * https://oeis.org/A248999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 20, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A248999
   A(n) = number of 0‑6 arrays of length n+4 with no five consecutive
   terms such that two times the sum of any three elements equals three
   times the sum of the remaining two.
   Computation: dynamic programming with a finite‑state automaton.
   State = last four symbols (a,b,c,d).  The transition from a state
   to a new state (b,c,d,e) is allowed iff the five‑tuple
   (a,b,c,d,e) does not contain a forbidden combination.
   The DP runs in O(n) time with a state space of 7⁴ = 2401.  */

#include <stdbool.h>
#include <stdint.h>

#define STATE_COUNT (7 * 7 * 7 * 7)

/* Pre‑compute transition validity and target state once. */
static bool  valid[STATE_COUNT][7];
static uint16_t next_state[STATE_COUNT][7];
static bool transitions_inited = false;

/* Helper: map a quadruple to a state index. */
static inline int state_index(int a, int b, int c, int d)
{
    return ((a * 7 + b) * 7 + c) * 7 + d;
}

/* Build transition tables on first call. */
static void init_transitions(void)
{
    if (transitions_inited) return;
    const int combos[10][3] = {
        {0,1,2},{0,1,3},{0,1,4},
        {0,2,3},{0,2,4},{0,3,4},
        {1,2,3},{1,2,4},{1,3,4},
        {2,3,4}
    };
    for (int a = 0; a < 7; ++a) {
        for (int b = 0; b < 7; ++b) {
            for (int c = 0; c < 7; ++c) {
                for (int d = 0; d < 7; ++d) {
                    int st = state_index(a,b,c,d);
                    for (int e = 0; e < 7; ++e) {
                        int total = a + b + c + d + e;
                        bool bad = false;
                        if ((total % 5) == 0) {
                            int target = 3 * total / 5;
                            int arr[5] = {a,b,c,d,e};
                            for (int k = 0; k < 10 && !bad; ++k) {
                                int sum = arr[combos[k][0]] +
                                          arr[combos[k][1]] +
                                          arr[combos[k][2]];
                                if (sum == target) bad = true;
                            }
                        }
                        valid[st][e] = !bad;
                        next_state[st][e] = state_index(b,c,d,e);
                    }
                }
            }
        }
    }
    transitions_inited = true;
}

/* Main sequence function requested by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_transitions();

    /* DP arrays: two rows, one for current counts, one for next. */
    static seq_t dp[2][STATE_COUNT];
    seq_t *dp_cur = dp[0];
    seq_t *dp_next = dp[1];

    /* Initialise for sequences of length 4 (i.e., one step already taken). */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 1;

    /* We need to extend the length from 4 to n+4, i.e., n additional symbols. */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear next row. */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* Transition from all current states. */
        for (int st = 0; st < STATE_COUNT; ++st) {
            seq_t cur_cnt = dp_cur[st];
            if (cur_cnt == 0) continue;
            for (int e = 0; e < 7; ++e) {
                if (!valid[st][e]) continue;
                int ns = next_state[st][e];
                dp_next[ns] += cur_cnt;
            }
        }
        /* Swap pointers for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all sequences of the full length. */
    seq_t ans = 0;
    for (int i = 0; i < STATE_COUNT; ++i) ans += dp_cur[i];
    return ans;
}
