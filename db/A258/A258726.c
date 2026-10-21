/*
 * A258726  Number of length n+4 0..3 arrays with at most one downstep in
 *          every 4 consecutive neighbor pairs.
 * https://oeis.org/A258726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 37, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A258726: Number of length n+4 arrays over {0,1,2,3}
 * with at most one downstep in every 4 consecutive neighbor pairs.
 * Computed by dynamic programming over the last element value and the
 * last three flags indicating downsteps; the DP guarantees every
 * window of 4 consecutive flags has sum ≤ 1.
 */
#include <stdbool.h>

/* popcount for 3‑bit numbers */
static const int POP3[8] = {0,1,1,2,1,2,2,3};

seq_t a(idx_t n)
{
    /* length of the arrays */
    idx_t N = n + 4;

    /* DP tables: 4 (values) × 8 (3‑bit flags) = 32 states */
    seq_t dp[32] = {0};
    seq_t ndp[32] = {0};

    /* initial state: first element can be anything, no flags yet */
    for (int v = 0; v < 4; ++v)
        dp[(v << 3) | 0] = 1;

    /* add remaining N‑1 elements */
    for (idx_t pos = 1; pos < N; ++pos) {
        /* clear next DP table */
        for (int i = 0; i < 32; ++i)
            ndp[i] = 0;

        /* process current states */
        for (int prev = 0; prev < 4; ++prev) {
            for (int bits = 0; bits < 8; ++bits) {
                int state = (prev << 3) | bits;
                seq_t cnt = dp[state];
                if (cnt == 0)   /* skip empty state */
                    continue;

                /* try all next values */
                for (int next = 0; next < 4; ++next) {
                    int d = (prev > next) ? 1 : 0;
                    int new_bits = ((bits << 1) | d) & 7;   /* keep last 3 flags */

                    /* enforce window rule when we have at least 4 flags */
                    if (pos >= 4) {
                        if (POP3[bits] + d > 1)
                            continue;
                    }

                    int new_state = (next << 3) | new_bits;
                    ndp[new_state] += cnt;
                }
            }
        }

        /* swap dp and ndp for next iteration */
        for (int i = 0; i < 32; ++i)
            dp[i] = ndp[i];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int i = 0; i < 32; ++i)
        result += dp[i];

    return result;
}
