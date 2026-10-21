/*
 * A241961  Number of length n+3 0..5 arrays with no consecutive four elements
 *          summing to more than 2*5.
 * https://oeis.org/A241961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+3 0..5 arrays with no consecutive 4
 *            elements summing to more than 10
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const int STATE_COUNT = 216;   /* 6^3 possible last-three patterns */
    const int ALPHABET    = 6;     /* digits 0 .. 5 */
    idx_t N = n + 3;               /* total length of arrays to be counted */

    seq_t dp[2][STATE_COUNT];
    int cur = 0, nxt = 1;

    /* Base: all 3‑element prefixes are allowed (no restriction yet) */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp[cur][i] = 1;

    /* Extend the array one element at a time, for positions 4 .. N */
    for (idx_t pos = 4; pos <= N; ++pos) {
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[nxt][i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[cur][s];
            if (!val) continue;   /* skip zero counts */

            /* Decode the current 3‑tuple (a,b,c) from state index s */
            int a = s / 36;
            int bc = s % 36;
            int b = bc / 6;
            int c = bc % 6;
            int sum3 = a + b + c;

            /* Try all possible next digits d (0..5) */
            for (int d = 0; d < ALPHABET; ++d) {
                if (sum3 + d <= 10) {            /* window of 4 satisfies the bound */
                    int ns = b * 36 + c * 6 + d;  /* new state after appending d */
                    dp[nxt][ns] += val;
                }
            }
        }
        /* Swap current and next DP arrays for the next iteration */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum over all states to get the total number of length N arrays */
    seq_t result = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        result += dp[cur][i];

    return result;
}
