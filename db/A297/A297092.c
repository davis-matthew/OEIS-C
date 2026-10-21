/*
 * A297092  Number of nX6 0..1 arrays with no 1 adjacent to 3 king-move
 *          neighboring 1s.
 * https://oeis.org/A297092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297092: Number of n×6 0..1 arrays with no 1 adjacent to 3 king‑move neighbouring 1s.
   The algorithm uses a transfer–matrix dynamic programming over two successive rows.
   Each state is a pair of 6‑bit rows (12 bits total).  For each state we pre‑compute the
   set of compatible next rows that keep the middle row valid.  After processing n
   real rows we add a virtual zero row below the last real row and sum the
   corresponding counts.  Complexity: O(n · S · T) with S = 2¹² = 4096 states and
   average outgoing edges T ≈ 10 – 15.  The routine works for any size provided by
   the build (uint8_t …, uint64_t, arbitrary‑precision, etc.). */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑computed transition tables */
#define STATES (1U << 12)       /* 2^12  = 4096  */
#define MAX_OUT 64              /* maximum outgoing edges per state */

static uint8_t tran_next[STATES][MAX_OUT] = {0};
static uint8_t tran_len[STATES] = {0};
static uint8_t valid_last[STATES] = {0};

static bool precomp_done = false;

/*--------------------------------------------------------------------------*/
/*  Check whether the middle row (mid) is valid given its upper (top)
    and lower (bottom) neighbours. */
static bool valid_row(uint8_t mid, uint8_t top, uint8_t bot)
{
    for (int j = 0; j < 6; ++j)
    {
        if (!((mid >> j) & 1))
            continue;                     /* only need to check cells that are 1 */

        int neigh = 0;

        /* Top row neighbours (include (i-1,j) and the two diagonals) */
        if (j > 0)   neigh += (top >> (j - 1)) & 1;
        neigh      += (top >> j) & 1;
        if (j < 5)   neigh += (top >> (j + 1)) & 1;

        /* Middle row neighbours (exclude the cell itself) */
        if (j > 0)   neigh += (mid >> (j - 1)) & 1;
        if (j < 5)   neigh += (mid >> (j + 1)) & 1;

        /* Bottom row neighbours (include (i+1,j) and the two diagonals) */
        if (j > 0)   neigh += (bot >> (j - 1)) & 1;
        neigh      += (bot >> j) & 1;
        if (j < 5)   neigh += (bot >> (j + 1)) & 1;

        if (neigh == 3)           /* forbidden configuration */
            return false;
    }
    return true;
}

/*--------------------------------------------------------------------------*/
/*  Create the transition tables once. */
static void precompute_transitions(void)
{
    if (precomp_done)
        return;

    for (int top = 0; top < 64; ++top)
    {
        for (int mid = 0; mid < 64; ++mid)
        {
            int state = (top << 6) | mid;
            int cnt = 0;
            bool last_ok = false;

            for (int bot = 0; bot < 64; ++bot)
            {
                if (!valid_row((uint8_t)mid, (uint8_t)top, (uint8_t)bot))
                    continue;

                tran_next[state][cnt++] = (uint8_t)bot;
                if (bot == 0)
                    last_ok = true;
            }
            tran_len[state] = (uint8_t)cnt;
            valid_last[state] = last_ok ? 1 : 0;
        }
    }
    precomp_done = true;
}

/*--------------------------------------------------------------------------*/
/*  Main sequence function.  (idx_t and seq_t are defined by the build.) */
seq_t a(idx_t n)
{
    precompute_transitions();

    /* DP arrays */
    static seq_t dp[STATES];
    static seq_t ndp[STATES];
    int i, s, k;

    /* initialise dp with state (0,0) = 1 */
    for (s = 0; s < STATES; ++s)
        dp[s] = 0;
    dp[0] = 1;

    /* process each of the n real rows */
    for (i = 0; i < n; ++i)
    {
        /* zero ndp */
        for (s = 0; s < STATES; ++s)
            ndp[s] = 0;

        for (s = 0; s < STATES; ++s)
        {
            seq_t val = dp[s];
            if (val == 0)
                continue;

            int mid = s & 63;            /* lower 6 bits: current middle row */
            int len = tran_len[s];

            for (k = 0; k < len; ++k)
            {
                int bot = tran_next[s][k];
                int new_state = (mid << 6) | bot;   /* new pair (mid,bot) */
                ndp[new_state] += val;
            }
        }
        /* swap dp and ndp */
        for (s = 0; s < STATES; ++s)
            dp[s] = ndp[s];
    }

    /* finalize by adding the virtual zero row below the last one */
    seq_t ans = 0;
    for (s = 0; s < STATES; ++s)
    {
        seq_t val = dp[s];
        if (val == 0)
            continue;
        if (valid_last[s])
            ans += val;
    }
    return ans;
}
