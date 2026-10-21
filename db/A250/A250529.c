/*
 * A250529  Number of (n+1)X(1+1) 0..3 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 tables with entries 0..3 satisfying *
 *            (i) max(x(i,1),x(i,0)) is non‑decreasing as i increases, * (ii)
 *            min(x(i,1),x(i-1,1)) ≥ min(x(i,0),x(i-1,0)) for all i≥1. *
 *            Computed by dynamic programming on 16 states (a_i,b_i) ∈ {0..3}²
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* ------------------------------------------------------------ */
/* Transition matrix: from state (a,b) to state (a',b')             */
/* ------------------------------------------------------------ */
static char trans[16][16];
static int trans_ready = 0;

static void init_trans(void)
{
    for (int s = 0; s < 16; ++s) {
        int a  = s >> 2;     /* a = s/4 */
        int b  = s & 3;      /* b = s%4 */
        for (int t = 0; t < 16; ++t) {
            int ap = t >> 2;   /* a' */
            int bp = t & 3;    /* b' */
            /* condition 1: max(a',b') >= max(a,b) */
            int Mprev = a > b ? a : b;
            int Mnext = ap > bp ? ap : bp;
            if (Mnext < Mprev) { trans[s][t] = 0; continue; }
            /* condition 2: min(b',b) >= min(a',a) */
            int minA = ap < a ? ap : a;
            int minB = bp < b ? bp : b;
            if (minB < minA) { trans[s][t] = 0; continue; }
            trans[s][t] = 1;
        }
    }
    trans_ready = 1;
}

/* ------------------------------------------------------------ */
/* Main function required by the judge                            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n≥1 */

    if (!trans_ready) init_trans();

    /* dp[s] = number of ways to reach state s after current number of rows */
    seq_t dp[16];
    seq_t ndp[16];

    for (int i = 0; i < 16; ++i) dp[i] = (seq_t)1;   /* initial row free */

    /* We need to perform n transitions (from row 0 to row n) */
    for (idx_t step = 0; step < n; ++step) {
        for (int t = 0; t < 16; ++t) ndp[t] = (seq_t)0;
        for (int s = 0; s < 16; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            for (int t = 0; t < 16; ++t) {
                if (trans[s][t]) ndp[t] += cur;
            }
        }
        /* copy ndp -> dp for next iteration */
        for (int i = 0; i < 16; ++i) dp[i] = ndp[i];
    }

    /* Sum all states to get the total number of arrays */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 16; ++i) total += dp[i];
    return total;
}
