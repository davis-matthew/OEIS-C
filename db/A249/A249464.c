/*
 * A249464  Number of length n+4 0..6 arrays with no five consecutive terms
 *          having four times any element equal to the sum of the remaining
 *          four.
 * https://oeis.org/A249464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays over {0,…,6} in which no
 *            5‑consecutive terms contain an element equal to the average of
 *            those five terms. This is equivalent to forbidding any 5‑term
 *            window (a,b,c,d,e) with 5·a = a+b+c+d+e, 5·b = …, or 5·e = …
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define TRANS_LEN 2401  /* 7^4 */

seq_t a(idx_t n)
{
    /* Precomputation of transitions (done once). */
    static int init = 0;
    static int trans_next[TRANS_LEN][7];
    static unsigned char trans_cnt[TRANS_LEN];
    if (!init) {
        unsigned char allowed[16807];
        /* All windows initially allowed. */
        for (int i = 0; i < 16807; ++i) allowed[i] = 1;

        /* Mark forbidden windows. */
        for (int a = 0; a < 7; ++a)
            for (int b = 0; b < 7; ++b)
                for (int c = 0; c < 7; ++c)
                    for (int d = 0; d < 7; ++d)
                        for (int e = 0; e < 7; ++e) {
                            int sum = a + b + c + d + e;
                            int idx = (((((a*7)+b)*7 + c)*7 + d)*7 + e);
                            if (sum % 5 == 0) {
                                int avg = sum / 5;
                                if (a==avg || b==avg || c==avg || d==avg || e==avg)
                                    allowed[idx] = 0;
                            }
                        }

        /* Build transition lists. */
        for (int a = 0; a < 7; ++a)
            for (int b = 0; b < 7; ++b)
                for (int c = 0; c < 7; ++c)
                    for (int d = 0; d < 7; ++d) {
                        int state = ((((a*7)+b)*7 + c)*7 + d);
                        int cnt = 0;
                        for (int e = 0; e < 7; ++e) {
                            int sum = a + b + c + d + e;
                            int idx = (((((a*7)+b)*7 + c)*7 + d)*7 + e);
                            if (allowed[idx]) {
                                int next = ((((b*7)+c)*7 + d)*7 + e);
                                trans_next[state][cnt++] = next;
                            }
                        }
                        trans_cnt[state] = cnt;
                    }
        init = 1;
    }

    /* DP arrays.  They are static so that they survive between calls. */
    static seq_t dp[TRANS_LEN];
    static seq_t nextdp[TRANS_LEN];

    /* Initialise DP with all 4‑tuples.  Each corresponds to 1 sequence. */
    for (int i = 0; i < TRANS_LEN; ++i) dp[i] = 0;
    for (int a = 0; a < 7; ++a)
        for (int b = 0; b < 7; ++b)
            for (int c = 0; c < 7; ++c)
                for (int d = 0; d < 7; ++d) {
                    int state = ((((a*7)+b)*7 + c)*7 + d);
                    dp[state] = 1;
                }

    /* Extend the arrays n times (adding one element each time). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < TRANS_LEN; ++i) nextdp[i] = 0;
        for (int s = 0; s < TRANS_LEN; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int cnt = trans_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                int ns = trans_next[s][k];
                nextdp[ns] += val;
            }
        }
        /* Swap dp and nextdp. */
        for (int i = 0; i < TRANS_LEN; ++i) dp[i] = nextdp[i];
    }

    /* Sum all dp entries to obtain the total number of valid arrays. */
    seq_t total = 0;
    for (int i = 0; i < TRANS_LEN; ++i) total += dp[i];
    return total;
}
