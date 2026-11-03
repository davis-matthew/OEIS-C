/*
 * A384154  a(n) is the number of binary strings of length n whose shortest
 *          run of 1s is of length 2.
 * https://oeis.org/A384154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 821 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t count_min_run(idx_t n, int k) {
    /* dp[c] = number of valid strings of current length with a trailing
       run of 1s of length c; c ranges 0 .. k (k means length >= k). */
    seq_t cur[4] = {0};  /* k is at most 3, enough for A384154 */
    cur[0] = 1;          /* empty string has trailing run length 0 */
    for (int c = 1; c <= k; ++c) cur[c] = 0;

    for (idx_t pos = 0; pos < n; ++pos) {
        seq_t next[4] = {0};
        for (int c = 0; c <= k; ++c) {
            seq_t val = cur[c];
            if (!val) continue;
            if (c == 0) {
                /* add 0 -> stay in state 0 */
                next[0] += val;
                /* add 1 -> state 1 */
                next[1] += val;
            } else if (c < k) {
                /* add 1 -> state c+1 */
                next[c + 1] += val;
            } else { /* c == k */
                /* add 0 -> state 0 */
                next[0] += val;
                /* add 1 -> stay in state k */
                next[k] += val;
            }
        }
        for (int c = 0; c <= k; ++c) cur[c] = next[c];
    }
    return cur[0] + cur[k];   /* only states that end with no incomplete run */
}
/* Computes a(n) = number of binary strings of length n whose
   shortest run of 1s is of length 2.
   Implementation counts strings where all runs of 1s have length >=2
   (call this S2) and strings where all runs have length >=3
   (call this S3).  Then a(n)=S2-S3.  The counts are obtained by a
   simple DP that keeps track of the current run length of 1s. */

seq_t a(idx_t n) {
    seq_t g2 = count_min_run(n, 2);
    seq_t g3 = count_min_run(n, 3);
    return g2 - g3;
}
