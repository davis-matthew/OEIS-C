/*
 * A250704  Number of (n+1) X (5+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 6 arrays with entries 0..3 satisfying *
 *            1. for every j=1..5 : x(i,j)+x(i,j-1) is non‑decreasing in i, *
 *            2. for every i : min(x(i,j),x(i-1,j)) is non‑decreasing in j. *
 *            The sequence is computed by modelling each row of 6 entries as a
 *            node * (4096 possible nodes). Transitio...
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 24, u128 n <= 110,
 *            big any n
 * Helpers:   digit, ok_transition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit.h"
#include "../utils/ok_transition.h"

#define MAX_STATES 4096   /* 4^6 */

static bool initialized = false;
static int  *edge_prev = NULL;
static int  *edge_curr = NULL;
static int   edge_cnt   = 0;
static seq_t *dp  = NULL;
static seq_t *dp_new = NULL;

/* Build the adjacency lists (edges) once. */
static void init_graph(void)
{
    /* First pass: count edges. */
    for (int p = 0; p < MAX_STATES; ++p) {
        for (int c = 0; c < MAX_STATES; ++c) {
            if (ok_transition(p, c))
                ++edge_cnt;
        }
    }

    /* Allocate arrays for edges. */
    edge_prev = (int *)malloc(sizeof(int) * edge_cnt);
    edge_curr = (int *)malloc(sizeof(int) * edge_cnt);
    if (!edge_prev || !edge_curr) {
        /* Memory allocation failure – abort. */
        exit(1);
    }

    /* Second pass: store edges. */
    int idx = 0;
    for (int p = 0; p < MAX_STATES; ++p) {
        for (int c = 0; c < MAX_STATES; ++c) {
            if (ok_transition(p, c)) {
                edge_prev[idx] = p;
                edge_curr[idx] = c;
                ++idx;
            }
        }
    }

    /* Allocate DP arrays. */
    dp      = (seq_t *)malloc(sizeof(seq_t) * MAX_STATES);
    dp_new  = (seq_t *)malloc(sizeof(seq_t) * MAX_STATES);
    if (!dp || !dp_new) {
        exit(1);
    }

    initialized = true;
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    if (!initialized)
        init_graph();

    /* Initialize dp for step 0: one way to start at any node. */
    for (int i = 0; i < MAX_STATES; ++i)
        dp[i] = (seq_t)1;

    /* Perform n transitions. */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero dp_new. */
        for (int i = 0; i < MAX_STATES; ++i)
            dp_new[i] = (seq_t)0;

        /* Accumulate transitions. */
        for (int e = 0; e < edge_cnt; ++e) {
            int p = edge_prev[e];
            int c = edge_curr[e];
            dp_new[c] += dp[p];
        }

        /* Swap dp and dp_new for next iteration. */
        seq_t *tmp = dp;
        dp = dp_new;
        dp_new = tmp;
    }

    /* Sum all dp values: total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < MAX_STATES; ++i)
        total += dp[i];

    return total;
}
