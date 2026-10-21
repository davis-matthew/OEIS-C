/*
 * A254694  Number of length n+4 0..4 arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays over {0,1,2,3,4} such that
 *            every consecutive block of five terms contains a pair whose
 *            maximum equals the minimum of the remaining three terms
 * Range:     u8 none, u16 n <= 4, u32 n <= 16, u64 n <= 37, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* State‑based DP: 4‑symbol states (625 states). */

#include <stdbool.h>

#define STATE_SIZE 625

/* Precomputation tables */
static bool              precomp_done = false;
static bool              allowed_transition[STATE_SIZE][5];
static int               next_state[STATE_SIZE][5];

/* ------------------------------------------------------------------ */
/* Precompute allowed transitions between 4‑symbol states               */
static void precompute(void)
{
    /* Enumerate all 5‑letter tuples (a0,a1,a2,a3,next) */
    for (int a0 = 0; a0 < 5; ++a0)
    for (int a1 = 0; a1 < 5; ++a1)
    for (int a2 = 0; a2 < 5; ++a2)
    for (int a3 = 0; a3 < 5; ++a3)
    {
        int state = a0 + 5*a1 + 25*a2 + 125*a3;
        for (int next = 0; next < 5; ++next)
        {
            /* Check the condition for the 5‑tuple */
            int v[5] = {a0, a1, a2, a3, next};
            bool ok = false;
            for (int i = 0; i < 5 && !ok; ++i)
            for (int j = i + 1; j < 5 && !ok; ++j)
            {
                int mx = v[i] > v[j] ? v[i] : v[j];
                int mn = 9;
                for (int k = 0; k < 5; ++k)
                    if (k != i && k != j && v[k] < mn) mn = v[k];
                if (mx == mn) ok = true;
            }
            allowed_transition[state][next] = ok;
            if (ok)
            {
                int ns = a1 + 5*a2 + 25*a3 + 125*next;   /* encode next 4 symbols */
                next_state[state][next] = ns;
            }
        }
    }
    precomp_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t a(idx_t n)
{
    if (!precomp_done) precompute();

    /* DP arrays: dp holds counts for current step, newdp for next step */
    seq_t dp[STATE_SIZE];
    seq_t newdp[STATE_SIZE];

    /* initial state: any 4‑symbol combination is allowed */
    for (idx_t i = 0; i < STATE_SIZE; ++i) dp[i] = (seq_t)1;

    /* Each of the n remaining symbols corresponds to a transition */
    for (idx_t step = 0; step < n; ++step)
    {
        /* zero newdp */
        for (idx_t i = 0; i < STATE_SIZE; ++i) newdp[i] = (seq_t)0;

        /* transition step */
        for (idx_t s = 0; s < STATE_SIZE; ++s)
        {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            for (int next = 0; next < 5; ++next)
            {
                if (!allowed_transition[s][next]) continue;
                int ns = next_state[s][next];
                newdp[ns] += cur;
            }
        }

        /* copy newdp back to dp for next iteration */
        for (idx_t i = 0; i < STATE_SIZE; ++i) dp[i] = newdp[i];
    }

    /* total count is the sum over all end states */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < STATE_SIZE; ++i) total += dp[i];
    return total;
}
