/*
 * A251000  Number of (6+1)X(n+1) 0..2 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A251000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 7×(n+1) arrays of 0..2 with nondecreasing
 *            min(x(i,j),x(i,j‑1)) in the i direction and nondecreasing
 *            |x(i,j)−x(i−1,j)| in the j direction
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 25, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Number of possible column states: 3^7 */
#define STATE_CNT 2187

/* Pre‑computed data structures (initialised once) */
static bool   init_done = false;
static uint8_t  state_vals[STATE_CNT][7];
static uint16_t edges_all[STATE_CNT * STATE_CNT];
static uint16_t degree[STATE_CNT];
static seq_t   dp[2][STATE_CNT];

/* One‑time initialisation of the transition graph */
static void init(void)
{
    int s, i, prev, curr;
    /* Enumerate all 7‑tuples of values 0..2 */
    for (s = 0; s < STATE_CNT; ++s) {
        int r = s;
        for (i = 0; i < 7; ++i) {
            state_vals[s][i] = r % 3;
            r /= 3;
        }
    }

    /* Compute allowed transitions */
    for (prev = 0; prev < STATE_CNT; ++prev)
        degree[prev] = 0;

    for (prev = 0; prev < STATE_CNT; ++prev) {
        for (curr = 0; curr < STATE_CNT; ++curr) {
            bool ok = true;

            /* i‑direction: min(x(i,j),x(i,j‑1)) is non‑decreasing */
            for (i = 1; i < 7 && ok; ++i) {
                uint8_t a = (state_vals[curr][i] < state_vals[prev][i]) ?
                            state_vals[curr][i] : state_vals[prev][i];
                uint8_t b = (state_vals[curr][i-1] < state_vals[prev][i-1]) ?
                            state_vals[curr][i-1] : state_vals[prev][i-1];
                if (a < b) ok = false;
            }

            /* j‑direction: |x(i,j)−x(i‑1,j)| is non‑decreasing */
            for (i = 1; i < 7 && ok; ++i) {
                int diffCurr = (int)state_vals[curr][i] - (int)state_vals[curr][i-1];
                if (diffCurr < 0) diffCurr = -diffCurr;
                int diffPrev = (int)state_vals[prev][i] - (int)state_vals[prev][i-1];
                if (diffPrev < 0) diffPrev = -diffPrev;
                if (diffCurr < diffPrev) ok = false;
            }

            if (ok) {
                edges_all[prev * STATE_CNT + degree[prev]] = (uint16_t)curr;
                degree[prev]++;
            }
        }
    }

    init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!init_done) init();

    int cur = 0, nxt = 1;

    /* initialise dp[cur] = 1 for every starting column */
    for (int s = 0; s < STATE_CNT; ++s)
        dp[cur][s] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        /* zero dp[nxt] */
        for (int s = 0; s < STATE_CNT; ++s)
            dp[nxt][s] = (seq_t)0;

        /* transition */
        for (int prev = 0; prev < STATE_CNT; ++prev) {
            seq_t v = dp[cur][prev];
            if (v == (seq_t)0) continue;
            uint16_t *vec = &edges_all[prev * STATE_CNT];
            uint16_t d = degree[prev];
            for (uint16_t k = 0; k < d; ++k) {
                uint16_t curr_state = vec[k];
                dp[nxt][curr_state] += v;
            }
        }

        /* swap current / next */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum all counts in the final column */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < STATE_CNT; ++s)
        ans += dp[cur][s];

    return ans;
}
