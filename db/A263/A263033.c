/*
 * A263033  Number of (n+2)X(1+2) 0..1 arrays with each row and column not
 *          divisible by 5, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+2)×3 binary arrays with each row and column not
 *            divisible by 5. Computed by DP over column remainder states
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

seq_t a(idx_t n) {
    /* maximum number of rows is n+2; each column remainder is 0..4,
       so we have 5^3 = 125 states. */
    static seq_t dp1[125];
    static seq_t dp2[125];
    seq_t *cur = dp1, *nxt = dp2;
    const seq_t *valid_masks[6] = {
        /* masks 0..7, but only those with row value %5 != 0 are used. */
    };
    /* Precompute valid row patterns (0..7) excluding those divisible by 5. */
    static int row_masks[6] = {1,2,3,4,6,7}; /* 0 and 5 omitted */
    const int pattern_count = 6;

    /* Initialize DP: before any rows, all remainders are 0. */
    memset(cur, 0, sizeof(dp1));
    cur[0] = 1;

    /* transition for each row */
    for (idx_t r = 1; r <= n + 2; ++r) {
        memset(nxt, 0, sizeof(dp2));
        for (int idx = 0; idx < 125; ++idx) {
            seq_t ways = cur[idx];
            if (!ways) continue;
            /* decode current remainders */
            int s0 = idx / 25;
            int rem = idx % 25;
            int s1 = rem / 5;
            int s2 = rem % 5;
            for (int p = 0; p < pattern_count; ++p) {
                int mask = row_masks[p];
                /* bits for columns: b0 = leftmost, b1, b2 rightmost */
                int b0 = (mask >> 2) & 1;
                int b1 = (mask >> 1) & 1;
                int b2 = mask & 1;
                int ns0 = ( (s0 << 1) + b0 ) % 5; /* s0*2 + b0 */
                int ns1 = ( (s1 << 1) + b1 ) % 5;
                int ns2 = ( (s2 << 1) + b2 ) % 5;
                int nidx = ns0 * 25 + ns1 * 5 + ns2;
                nxt[nidx] += ways;
            }
        }
        /* swap arrays for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum counts where all column remainders are non‑zero. */
    seq_t ans = 0;
    for (int idx = 0; idx < 125; ++idx) {
        int s0 = idx / 25;
        int rem = idx % 25;
        int s1 = rem / 5;
        int s2 = rem % 5;
        if (s0 && s1 && s2) {
            ans += cur[idx];
        }
    }
    return ans;
}
