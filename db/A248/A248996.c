/*
 * A248996  Number of length n+4 0..3 arrays with no five consecutive terms
 *          having two times the sum of any three elements equal to three
 *          times the sum of the remaining two.
 * https://oeis.org/A248996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 strings over {0,1,2,3} such that in
 *            every * 5 consecutive positions there is no subset of 3 whose
 *            twice the sum equals * three times the sum of the remaining 2.
 *            This is computed by dynamic * programming over the 256 possible
 *            4‑tuples. No hard‑coded terms are used
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 32, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Tables that are built only once */
    static int tables_ready = 0;
    static unsigned char next_state[256][4];
    static unsigned char next_count[256];
    static unsigned char allowed_5[1024];

    if (!tables_ready) {
        /* Precompute which 5‑tuples are allowed */
        const int comb[10][3] = {
            {0,1,2},{0,1,3},{0,1,4},
            {0,2,3},{0,2,4},{0,3,4},
            {1,2,3},{1,2,4},{1,3,4},
            {2,3,4}
        };
        for (int a=0;a<4;a++)
        for (int b=0;b<4;b++)
        for (int c=0;c<4;c++)
        for (int d=0;d<4;d++)
        for (int e=0;e<4;e++) {
            int idx = (((((a*4)+b)*4)+c)*4+d)*4+e;
            int sum[5] = {a,b,c,d,e};
            int total = a+b+c+d+e;
            int ok = 1;
            for (int ci=0; ci<10 && ok; ++ci) {
                int s = sum[comb[ci][0]] + sum[comb[ci][1]] + sum[comb[ci][2]];
                int other = total - s;
                if (2*s == 3*other) ok = 0;
            }
            allowed_5[idx] = (unsigned char)ok;
        }

        /* Build adjacency lists for each 4‑tuple */
        for (int state=0; state<256; ++state) {
            int a = (state>>6)&3;
            int b = (state>>4)&3;
            int c = (state>>2)&3;
            int d = state&3;
            int cnt=0;
            for (int e=0; e<4; ++e) {
                int idx = (((((a*4)+b)*4)+c)*4+d)*4+e;
                if (!allowed_5[idx]) continue;
                int next = ((state<<2)&0xFF)|e;
                next_state[state][cnt] = (unsigned char)next;
                ++cnt;
            }
            next_count[state] = (unsigned char)cnt;
        }
        tables_ready = 1;
    }

    if (n == 0) return 0;          /* sequence defined from offset 1 */

    idx_t N = n + 4;                /* length of the strings */

    static seq_t dp[256];
    static seq_t dp_new[256];

    /* Initialize for length 4: all 4‑tuples are valid */
    for (int i=0; i<256; ++i) dp[i] = (seq_t)1;

    /* Extend to length N */
    for (idx_t len=5; len <= N; ++len) {
        memset(dp_new, 0, sizeof(dp_new));
        for (int s=0; s<256; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            unsigned char cnt = next_count[s];
            for (int j=0; j<cnt; ++j) {
                int t = next_state[s][j];
                dp_new[t] += val;
            }
        }
        memcpy(dp, dp_new, sizeof(dp));
    }

    /* Sum all ending states */
    seq_t total = 0;
    for (int s=0; s<256; ++s) total += dp[s];
    return total;
}
