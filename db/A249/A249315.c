/*
 * A249315  Number of length n+6 0..4 arrays with no seven consecutive terms
 *          having six times any element equal to the sum of the remaining
 *          six.
 * https://oeis.org/A249315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+6 arrays over {0…4} with no seven
 *            consecutive terms having 7 × a_i equal to the sum of that
 *            seven‑tuple. This is computed by a DFA with 5⁶ states
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 23, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Constants                                                             */
#define STATE_COUNT   15625          /* 5⁶ */
#define MOD5SEV       3125U          /* 5⁵ */

/* ------------------------------------------------------------------ */
/* Transition and validity tables (built once)                           */

/* next_state[s][d] gives the state after appending digit d to
   a state s.  allow[s][d] is true if appending d to s does not
   create a forbidden 7‑tuple. */
static unsigned short next_state[STATE_COUNT][5];
static bool          allow[STATE_COUNT][5];
static bool          init_done = false;

static void init_tables(void)
{
    if (init_done) return;

    for (int s = 0; s < STATE_COUNT; ++s) {
        unsigned a[6];
        int tmp = s;
        for (int i = 5; i >= 0; --i) { a[i] = tmp % 5; tmp /= 5; }

        unsigned sum6 = 0;
        for (int i = 0; i < 6; ++i) sum6 += a[i];

        for (int d = 0; d < 5; ++d) {
            unsigned sum7 = sum6 + d;
            bool bad = false;

            /* Check each position in the 7‑tuple */
            for (int i = 0; i < 6 && !bad; ++i)
                if (7 * a[i] == sum7) bad = true;
            if (!bad && 7 * d == sum7) bad = true;

            allow[s][d] = !bad;
            next_state[s][d] = (unsigned short)((s % MOD5SEV) * 5U + d);
        }
    }
    init_done = true;
}

/* ------------------------------------------------------------------ */
/* DP arrays (lazy allocation)                                         */
#ifdef __cplusplus
static seq_t *dp  = nullptr;
static seq_t *tmp = nullptr;
#else
static seq_t *dp  = NULL;
static seq_t *tmp = NULL;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1 */

    init_tables();

    /* Allocate DP arrays on first use */
    if (!dp) {
#ifdef __cplusplus
        dp  = new seq_t[STATE_COUNT];
        tmp = new seq_t[STATE_COUNT];
#else
        dp  = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
        tmp = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
#endif
        if (!dp || !tmp) return (seq_t)0; /* allocation failure */
    }

    /* Start with all 6‑digit sequences of length 6 (valid by definition). */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)1;

    /* Perform n transitions to reach length n+6. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i) tmp[i] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            for (int d = 0; d < 5; ++d) {
                if (!allow[s][d]) continue;
                int ns = next_state[s][d];
                tmp[ns] += cur;
            }
        }
        /* swap dp and tmp */
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
    }

    /* Sum all valid sequences of length n+6. */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i) ans += dp[i];

    return ans;
}
