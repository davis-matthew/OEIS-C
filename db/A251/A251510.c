/*
 * A251510  Number of (n+1)X(3+1) 0..4 arrays with every 2X2 subblock summing
 *          to a prime.
 * https://oeis.org/A251510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A251510: Number of (n+1)×4 0..4 arrays with every 2×2 subblock summing to a prime.
   Computation by dynamic programming over all row patterns.  
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define N_STATES 625  /* 5^4, all possible rows of length 4 */

static const bool prime_sum[17] = {
    /* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 */
    false, false, true, true, false,
    true,  false, true, false, false,
    false, true,  false, true, false,
    false, false
};

/* Store the base‑5 digits of each state */
static int state_dig[N_STATES][4];

/* Adjacency for transitions from one row to the next */
static int *adj_list[N_STATES];
static size_t *adj_len;
static size_t *adj_cap;

/* ------------------------------------------------------------------
   Add an edge between two states (used by init)
   ------------------------------------------------------------------ */
static void add_edge(int from, int to)
{
    if (adj_cap[from] == 0) {
        adj_cap[from] = 8;
        adj_list[from] = (int *)malloc(adj_cap[from] * sizeof(int));
    } else if (adj_len[from] == adj_cap[from]) {
        adj_cap[from] <<= 1;
        adj_list[from] = (int *)realloc(adj_list[from], adj_cap[from] * sizeof(int));
    }
    adj_list[from][adj_len[from]++] = to;
}

/* ------------------------------------------------------------------
   One‑time global initialisation
   ------------------------------------------------------------------ */
static void init(void)
{
    adj_len = (size_t *)calloc(N_STATES, sizeof(size_t));
    adj_cap = (size_t *)calloc(N_STATES, sizeof(size_t));

    /* Digit decomposition of every state */
    for (int s = 0; s < N_STATES; ++s) {
        int x = s;
        for (int i = 3; i >= 0; --i) {
            state_dig[s][i] = x % 5;
            x /= 5;
        }
    }

    /* Build the adjacency graph */
    for (int from = 0; from < N_STATES; ++from) {
        for (int to = 0; to < N_STATES; ++to) {
            bool ok = true;
            for (int c = 0; c < 3; ++c) {
                int sum = state_dig[from][c] + state_dig[from][c + 1] +
                          state_dig[to][c] + state_dig[to][c + 1];
                if (!prime_sum[sum]) { ok = false; break; }
            }
            if (ok) add_edge(from, to);
        }
    }
}

/* ------------------------------------------------------------------
   The sequence function (external linkage)
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init(); initialized = true; }

    /* Two layers of dynamic programming */
    seq_t dp[2][N_STATES];
    int cur = 0, next = 1;

    /* First row can be anything */
    for (int i = 0; i < N_STATES; ++i) dp[cur][i] = 1;

    /* Perform n transitions (we have n+1 rows) */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero next layer */
        for (int i = 0; i < N_STATES; ++i) dp[next][i] = 0;

        for (int from = 0; from < N_STATES; ++from) {
            seq_t val = dp[cur][from];
            if (val == 0) continue;
            size_t len = adj_len[from];
            for (size_t k = 0; k < len; ++k) {
                int to = adj_list[from][k];
                dp[next][to] += val;
            }
        }
        /* Swap layers */
        int tmp = cur; cur = next; next = tmp;
    }

    /* Sum all final states */
    seq_t total = 0;
    for (int i = 0; i < N_STATES; ++i) total += dp[cur][i];
    return total;
}
