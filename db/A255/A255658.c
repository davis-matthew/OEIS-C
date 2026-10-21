/*
 * A255658  Number of length n+6 0..3 arrays with at most two downsteps in
 *          every 6 consecutive neighbor pairs.
 * https://oeis.org/A255658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+6 0..3 arrays with at most two *
 *            down‑steps in any 6 consecutive neighbour pairs
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 29, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Number of down steps in the last 5 transitions (mask bits 0..4):
 * bit i corresponds to transition between positions i and i+1 (0 = oldest). */
static const unsigned char popcnt5[32] = {
    0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5
};

seq_t a(idx_t n)
{
    /* DP state: 4 possible last values × 32 possible masks of last 5
     * transitions (total 128 states).  dp[s] holds the number of
     * prefixes of current length ending in state s.  A state is encoded
     * as:  state = last_value * 32 + mask.  */
    seq_t dp[128];
    seq_t newdp[128];
    int i, j, step;

    /* initialise dp with all 6‑element prefixes */
    for (i = 0; i < 128; ++i) dp[i] = (seq_t)0;
    /* generate all 4^6 = 4096 6‑length prefixes */
    for (int enc = 0; enc < 4096; ++enc) {
        int tmp = enc;
        int val[6];
        for (i = 5; i >= 0; --i) {        /* decode into values 0..3 */
            val[i] = tmp & 3;
            tmp >>= 2;
        }
        int mask = 0;
        for (i = 0; i < 5; ++i)          /* mask of first 5 transitions */
            if (val[i] > val[i + 1]) mask |= 1 << i;
        int last = val[5];
        int state = last * 32 + mask;
        dp[state] += (seq_t)1;
    }

    /* perform n steps (each step appends one element to the array) */
    for (step = 0; step < (int)n; ++step) {
        /* zero newdp */
        for (i = 0; i < 128; ++i) newdp[i] = (seq_t)0;

        for (i = 0; i < 128; ++i) {
            seq_t cnt = dp[i];
            if (cnt == (seq_t)0) continue;
            int last  = i / 32;     /* last value */
            int mask  = i % 32;     /* last 5 transitions */
            int pc    = popcnt5[mask];

            for (j = 0; j < 4; ++j) {                /* try next value */
                int is_down = (last > j) ? 1 : 0;
                if (pc + is_down > 2) continue;      /* constraint violated */

                int new_mask = (mask >> 1) | (is_down << 4); /* drop oldest,
                                                               add newest */
                int new_state = j * 32 + new_mask;
                newdp[new_state] += cnt;
            }
        }
        /* swap dp and newdp */
        for (i = 0; i < 128; ++i) dp[i] = newdp[i];
    }

    /* sum all states to obtain answer */
    seq_t result = (seq_t)0;
    for (i = 0; i < 128; ++i) result += dp[i];
    return result;
}
