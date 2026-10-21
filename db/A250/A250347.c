/*
 * A250347  Number of length n arrays x(i), i=1..n with x(i) in i..i+4 and no
 *          value appearing more than 2 times.
 * https://oeis.org/A250347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic‐program counting arrays x(1..n) with x(i) ∈ {i,…,i+4}
 *            and each value used at most twice. The DP keeps a sliding window
 *            of the five possible values that may still occur.
 *            state_counts[s][k] is the number of occurrences of the k‑th
 *            value (0≤k≤4) in the current window. Transition: increment a...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- includes ------------------------------------------------------- */
#include <stdlib.h>

/* ---- static tables and cache --------------------------------------- */
static seq_t *cache      = NULL;
static idx_t cache_size  = 0;
static idx_t last_n      = 0;

static seq_t dp[243];
static seq_t newdp[243];
static int state_counts[243][5];
static int next_state[243][5];
static int initialized   = 0;

/* ---- sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialize transition tables once */
    if (!initialized)
    {
        for (int s = 0; s < 243; ++s)
        {
            int x = s;
            for (int k = 0; k < 5; ++k)
                state_counts[s][k] = x % 3, x /= 3;

            for (int k = 0; k < 5; ++k)
            {
                if (state_counts[s][k] < 2)
                {
                    int newc[5];
                    for (int j = 0; j < 5; ++j)
                        newc[j] = state_counts[s][j];
                    ++newc[k];

                    int shifted[5];
                    for (int j = 0; j < 4; ++j)
                        shifted[j] = newc[j + 1];
                    shifted[4] = 0;

                    int t = shifted[0] +
                            shifted[1] * 3 +
                            shifted[2] * 9 +
                            shifted[3] * 27 +
                            shifted[4] * 81;
                    next_state[s][k] = t;
                }
                else
                    next_state[s][k] = -1;
            }
        }

        for (int s = 0; s < 243; ++s)
            dp[s] = 0;
        dp[0] = 1;            /* base state: all counts = 0 */
        initialized = 1;
    }

    if (n < 1)
        return 0;              /* offset is 1 */

    /* grow value cache if needed */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* compute values from last_n+1 up to n */
    for (idx_t i = last_n + 1; i <= n; ++i)
    {
        for (int s = 0; s < 243; ++s)
            newdp[s] = 0;

        for (int s = 0; s < 243; ++s)
        {
            seq_t val = dp[s];
            if (!val)
                continue;
            for (int k = 0; k < 5; ++k)
            {
                if (state_counts[s][k] < 2)
                {
                    int t = next_state[s][k];
                    newdp[t] += val;
                }
            }
        }

        seq_t total = 0;
        for (int s = 0; s < 243; ++s)
            total += newdp[s];
        cache[i] = total;

        for (int s = 0; s < 243; ++s)
            dp[s] = newdp[s];
    }

    last_n = n;
    return cache[n];
}
