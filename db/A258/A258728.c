/*
 * A258728  Number of length n+6 0..3 arrays with at most one downstep in
 *          every 6 consecutive neighbor pairs.
 * https://oeis.org/A258728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 17, u64 n <= 42, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of length n+6 0..3 arrays with at most one
   downstep in every 6 consecutive neighbour pairs.
   Algorithm: dynamic programming on the last five downstep flags
   and the current last array value.  All values are kept in the
   builtin type seq_t (unsigned integer or arbitrary precision).  */

#include <stddef.h>
#include <string.h>   /* for memset */

#define STATE_BITS   5
#define STATE_MASK   ((1 << STATE_BITS) - 1)   /* 0x1f */

/* dp[current][mask][last] holds the count of sequences of the
   processed prefix that end with value 'last' and whose last five
   pair‑down flags are encoded in 'mask'.  */
static seq_t dp[2][1 << STATE_BITS][4];

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; return 0 otherwise */
    if (n == 0)
        return 0;

    /* length of the array to be generated */
    idx_t length = n + 6;

    /* initialise dp for a single element (no pairs yet) */
    int cur = 0, nxt = 1;
    memset(dp[cur], 0, sizeof(dp[cur]));
    for (int val = 0; val < 4; ++val)
        dp[cur][0][val] = 1;    /* mask 0 – no downsteps seen yet */

    /* iterate over the remaining elements */
    for (idx_t step = 1; step < length; ++step) {
        memset(dp[nxt], 0, sizeof(dp[nxt]));
        for (int mask = 0; mask < (1 << STATE_BITS); ++mask) {
            for (int last = 0; last < 4; ++last) {
                seq_t cur_cnt = dp[cur][mask][last];
                if (cur_cnt == 0)
                    continue;
                for (int next = 0; next < 4; ++next) {
                    int down = (last > next) ? 1 : 0;
                    /* a new downstep is allowed only if no downstep
                       occurred in the previous 5 pairs */
                    if (down && mask != 0)
                        continue;
                    int new_mask = ((mask << 1) | down) & STATE_MASK;
                    dp[nxt][new_mask][next] += cur_cnt;
                }
            }
        }
        /* swap current and next for the next iteration */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum all counts for the final prefix length */
    seq_t result = 0;
    for (int mask = 0; mask < (1 << STATE_BITS); ++mask)
        for (int last = 0; last < 4; ++last)
            result += dp[cur][mask][last];

    return result;
}
