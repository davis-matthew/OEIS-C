/*
 * A274872  Number of set partitions of [n] into exactly eight blocks such
 *          that all odd elements are in blocks with an odd index and all even
 *          elements are in blocks with an even index.
 * https://oeis.org/A274872
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of set partitions of [n] into exactly eight blocks
 *            such that all odd elements are in blocks with an odd index and
 *            all even elements are in blocks with an even index
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 21, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n < 8 the offset of the sequence is not defined; return 0. */
    if (n < 8) return (seq_t)0;

    /* We keep two 5×5 tables: dp[o][e] = number of ways to process the first
       elements where o odd blocks have started and e even blocks have started. */
    const int MAX = 5;          /* indices 0..4 */
    seq_t dp[2][MAX][MAX];
    int cur = 0, nxt = 1;

    /* Initialize all values to 0. */
    for (int o = 0; o < MAX; ++o)
        for (int e = 0; e < MAX; ++e)
            dp[cur][o][e] = (seq_t)0;

    dp[cur][0][0] = (seq_t)1;   /* no elements processed yet */

    for (idx_t i = 1; i <= n; ++i) {
        /* Clear next table */
        for (int o = 0; o < MAX; ++o)
            for (int e = 0; e < MAX; ++e)
                dp[nxt][o][e] = (seq_t)0;

        int is_odd = (int)(i & 1);   /* 1 if odd, 0 if even */

        for (int o = 0; o < MAX; ++o) {
            for (int e = 0; e < MAX; ++e) {
                seq_t val = dp[cur][o][e];
                if ((seq_t)0 == val) continue;

                if (is_odd) {   /* current element is odd */
                    /* Option: start a new odd block: must be allowed by alternation
                       and we must still have room for another odd block. */
                    if (o == e && o < 4) {
                        dp[nxt][o + 1][e] += val;
                    }
                    /* Option: join an existing odd block. */
                    if (o > 0) {
                        dp[nxt][o][e] += val * (seq_t)o;
                    }
                } else {        /* current element is even */
                    /* Option: start a new even block. */
                    if (e == o - 1 && e < 4) {
                        dp[nxt][o][e + 1] += val;
                    }
                    /* Option: join an existing even block. */
                    if (e > 0) {
                        dp[nxt][o][e] += val * (seq_t)e;
                    }
                }
            }
        }
        /* Swap tables for next iteration */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* After processing all n elements, the answer is the number of ways
       where all 4 odd and 4 even blocks have started. */
    return dp[cur][4][4];
}
