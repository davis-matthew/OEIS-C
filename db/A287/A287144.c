/*
 * A287144  Number of partitions of n such that the absolute difference
 *          between any part i and the sum of all other parts not larger than
 *          i is not larger than two.
 * https://oeis.org/A287144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 5289, u32 n <= 9649, u64 n <= 9649,
 *            u128 n <= 9649, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A287144: Number of partitions of n such that for each part i
   the absolute difference between i and the sum of all other parts
   not larger than i is at most two.
   Computation is performed by dynamic programming on the reversed
   (non‑decreasing) partition representation. */
#include <stdlib.h>
#include <stddef.h>

/* internal helper structures ------------------------------------------- */
typedef struct {
    int last;    /* last element value in the current reversed prefix */
    seq_t ct;    /* number of ways to reach this state */
} StateEntry;

typedef struct {
    size_t count;                /* number of stored entries (≤5) */
    StateEntry entries[5];       /* limited by {S-2, S-1, S, S+1, S+2} */
} StateArray;

/* add or accumulate a state into dp[S] -------------------------------
   If an entry with the same last value exists, its count is increased.
   The function assumes dp[S] has been initialized with count=0. */
static void add_state(StateArray *sa, int last, seq_t val)
{
    for (size_t i = 0; i < sa->count; ++i)
    {
        if (sa->entries[i].last == last)
        {
            sa->entries[i].ct += val;
            return;
        }
    }
    /* new distinct last value */
    sa->entries[sa->count].last = last;
    sa->entries[sa->count].ct   = val;
    sa->count++;
}

/* main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* empty partition */

    /* allocate DP table for sums 1..n */
    StateArray *dp = (StateArray *)malloc((n + 1) * sizeof(StateArray));
    if (!dp) return (seq_t)0;          /* allocation failure handling */

    for (idx_t i = 1; i <= n; ++i) dp[i].count = 0;

    /* initial states: first element of reversed partition can be 1 or 2 */
    if (n >= 1) add_state(&dp[1], 1, (seq_t)1);
    if (n >= 2) add_state(&dp[2], 2, (seq_t)1);

    /* DP: build states for increasing total sums */
    for (idx_t S = 1; S <= n; ++S)
    {
        StateArray *cur = &dp[S];
        for (size_t idx = 0; idx < cur->count; ++idx)
        {
            int  last = cur->entries[idx].last;
            seq_t ways = cur->entries[idx].ct;

            /* next element r must satisfy |r - S| <= 2 and r >= last */
            for (int delta = -2; delta <= 2; ++delta)
            {
                int r = (int)S + delta;
                if (r < 1) continue;          /* positive parts only */
                if (r < last) continue;        /* non‑decreasing order */
                idx_t S2 = S + r;
                if (S2 > n) continue;          /* exceed target sum */

                add_state(&dp[S2], r, ways);
            }
        }
    }

    /* sum all ways reaching total sum n */
    seq_t result = 0;
    for (size_t idx = 0; idx < dp[n].count; ++idx)
        result += dp[n].entries[idx].ct;

    free(dp);
    return result;
}
