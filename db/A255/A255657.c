/*
 * A255657  Number of length n+5 0..3 arrays with at most two downsteps in
 *          every 5 consecutive neighbor pairs.
 * https://oeis.org/A255657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 29, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A255657
   Number of arrays of length n+5 over 0..3 with at most two downsteps
   in every 5 consecutive neighbor pairs.

   Computation is performed by dynamic programming.
   States are the last 5 values of the current array.
   For a state s\in[0,1024), let d0..d4 be its 5 digits base‑4.
   dropCount[s] = number of indices i with di>di+1 (i=0..3).
   When appending a new value e, the new state is
       s' = ((s % 256) * 4) + e
   and the window of 5 transitions has
       dropCount[s] + (d4 > e)
   which must not exceed 2.

   The sequence a(n) is the total number of states after n steps
   (i.e., after constructing length n+5).  The function is
   defined for n>=1, in accordance with the offset 1 used by OEIS.
*/

#include <stddef.h>

static int  dropCount[1024];
static int  baseState[1024];
static int  lastDigit[1024];
static int  precomputed = 0;

static void init_precomputation(void)
{
    if (precomputed) return;
    for (int s = 0; s < 1024; ++s)
    {
        int tmp = s;
        int d[5];
        for (int i = 4; i >= 0; --i) { d[i] = tmp % 4; tmp /= 4; }
        int cnt = 0;
        for (int i = 0; i < 4; ++i)
            if (d[i] > d[i + 1]) ++cnt;
        dropCount[s]  = cnt;
        baseState[s]  = (s % 256) * 4;
        lastDigit[s]  = s & 3;           /* s % 4 */
    }
    precomputed = 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* offset is 1 */

    init_precomputation();

    /* DP tables */
    seq_t dp[1024];
    seq_t next[1024];

    /* start with all length‑5 sequences (no constraints yet) */
    for (int i = 0; i < 1024; ++i) dp[i] = 1;

    /* perform n steps to reach length n+5 */
    for (idx_t step = 0; step < n; ++step)
    {
        for (int i = 0; i < 1024; ++i) next[i] = 0;

        for (int s = 0; s < 1024; ++s)
        {
            seq_t ways = dp[s];
            if (!ways) continue;

            int base   = baseState[s];
            int last   = lastDigit[s];
            int dcnt   = dropCount[s];

            for (int e = 0; e < 4; ++e)
            {
                if (dcnt + ((last > e) ? 1 : 0) <= 2)
                    next[base + e] += ways;
            }
        }

        /* copy next back to dp for next iteration */
        for (int i = 0; i < 1024; ++i) dp[i] = next[i];
    }

    /* sum all states to get total number of arrays of length n+5 */
    seq_t total = 0;
    for (int i = 0; i < 1024; ++i) total += dp[i];
    return total;
}
