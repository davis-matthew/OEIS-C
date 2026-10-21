/*
 * A260505  Number of binary words of length n with exactly one occurrence of
 *          subword 010 and exactly two occurrences of subword 101.
 * https://oeis.org/A260505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 38, u64 n <= 80, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260505:  
   a(n) = number of binary words of length n containing exactly one
   occurrence of subword 010 and exactly two occurrences of subword
   101.
   Computation by dynamic programming on a finite automaton. */

#include <string.h>

#define NUM_STATES (3 * 4 * 2 * 3) /* len (0..2), last2 (0..3), cnt010 (0..1), cnt101 (0..2) */

static inline int state_index(int len, int last, int cnt010, int cnt101) {
    return ((((len * 4) + last) * 2 + cnt010) * 3 + cnt101);
}

seq_t a(idx_t n) {
    seq_t dp[2][NUM_STATES] = {0};
    int cur = 0, next = 1;
    const seq_t ONE = (seq_t)1;

    /* initial state: no bits, no counts */
    dp[cur][state_index(0, 0, 0, 0)] = ONE;

    for (idx_t pos = 0; pos < n; ++pos) {
        /* clear next buffer */
        for (int i = 0; i < NUM_STATES; ++i) dp[next][i] = 0;

        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t val = dp[cur][s];
            if (!val) continue;

            /* decode state */
            int tmp = s;
            int cnt101 = tmp % 3; tmp /= 3;
            int cnt010 = tmp % 2; tmp /= 2;
            int last   = tmp % 4; tmp /= 4;
            int len    = tmp;

            /* try appending 0 and 1 */
            for (int bit = 0; bit < 2; ++bit) {
                int nlen  = (len < 2) ? len + 1 : 2;
                int nlast = ((last & 1) << 1) | bit;
                int ncnt010 = cnt010;
                int ncnt101 = cnt101;

                if (len == 2) {                    /* a new 3‑bit pattern is formed */
                    int pat = (last << 1) | bit;   /* last two bits + bit */
                    if (pat == 2) {                 /* 010 */
                        if (cnt010 == 1) continue;  /* would exceed the limit */
                        ncnt010 = 1;
                    } else if (pat == 5) {          /* 101 */
                        if (cnt101 == 2) continue;  /* would exceed the limit */
                        ncnt101++;
                    }
                }
                int ns = state_index(nlen, nlast, ncnt010, ncnt101);
                dp[next][ns] += val;
            }
        }
        /* swap buffers for next iteration */
        cur ^= 1; next ^= 1;
    }

    /* sum over all final states having exactly one 010 and two 101 */
    seq_t ans = 0;
    for (int s = 0; s < NUM_STATES; ++s) {
        seq_t val = dp[cur][s];
        if (!val) continue;
        int tmp = s;
        int cnt101 = tmp % 3; tmp /= 3;
        int cnt010 = tmp % 2; tmp /= 2;
        if (cnt010 == 1 && cnt101 == 2)
            ans += val;
    }
    return ans;
}
