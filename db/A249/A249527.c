/*
 * A249527  Number of length n+5 0..5 arrays with no six consecutive terms
 *          having five times any element equal to the sum of the remaining
 *          five.
 * https://oeis.org/A249527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A249527 – number of length n+5 arrays over {0..5}
    with no six consecutive terms having five times any element
    equal to the sum of the remaining five.
    We use a DFA DP over the last five symbols. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STATE_COUNT 7776  /* 6^5 */

static int transition_table[STATE_COUNT][6];
static int transition_ready = 0;

/* initialise the DFA transition table */
static void init_transition_table(void)
{
    if (transition_ready) return;
    for (int st = 0; st < STATE_COUNT; ++st) {
        /* decode state to 5 digits in base 6 */
        int d[5];
        int x = st;
        for (int k = 4; k >= 0; --k) {
            d[k] = x % 6;
            x /= 6;
        }
        for (int c = 0; c < 6; ++c) {
            /* form the 6‑tuple */
            int w[6];
            for (int k = 0; k < 5; ++k) w[k] = d[k];
            w[5] = c;

            /* test forbidden condition */
            int forbid = 0;
            for (int j = 0; !forbid && j < 6; ++j) {
                int sum = 0;
                for (int k = 0; k < 6; ++k)
                    if (k != j) sum += w[k];
                if (5 * w[j] == sum) forbid = 1;
            }
            if (forbid) {
                transition_table[st][c] = -1;
            } else {
                /* compute new state: shift left, append c */
                int ns = 0;
                for (int k = 0; k < 4; ++k) ns = ns * 6 + d[k + 1];
                ns = ns * 6 + c;
                transition_table[st][c] = ns;
            }
        }
    }
    transition_ready = 1;
}

/* DP arrays and memoised results */
static seq_t *dp_cur   = NULL;
static seq_t *dp_next  = NULL;
static seq_t *memo     = NULL;          /* memo[n] = a(n), 1-indexed */
static size_t memo_cap = 0;
static idx_t current_n = 0;             /* highest n computed so far */

static void init_dp(void)
{
    init_transition_table();
    dp_cur  = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    dp_next = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    /* start with all 5‑tuples: one way each */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = (seq_t)1;
    /* memo[1] will be computed after first step */
    memo_cap = 16;
    memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    current_n = 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe */

    if (!dp_cur) init_dp();

    while (current_n < n) {
        /* dp_next = 0 */
        memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));

        for (int st = 0; st < STATE_COUNT; ++st) {
            seq_t cnt = dp_cur[st];
            if (cnt == 0) continue;
            for (int c = 0; c < 6; ++c) {
                int ns = transition_table[st][c];
                if (ns >= 0) dp_next[ns] += cnt;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;

        /* total count for this step */
        seq_t total = 0;
        for (int st = 0; st < STATE_COUNT; ++st) total += dp_cur[st];

        /* store result in memo */
        idx_t pos = current_n + 1;
        if (pos >= memo_cap) {
            size_t new_cap = memo_cap ? memo_cap * 2 : 16;
            while (new_cap <= pos) new_cap *= 2;
            memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            memo_cap = new_cap;
        }
        memo[pos] = total;
        current_n = pos;
    }
    return memo[n];
}
