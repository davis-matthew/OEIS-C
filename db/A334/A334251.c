/*
 * A334251  a(n) is the number of binary (0,1) sequences of length n that have
 *          at most two zeros in a window of seven consecutive symbols.
 * https://oeis.org/A334251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP over the last 6 symbols.
   dp[mask]  – number of admissible sequences of current length whose
   right‑hand side contains the last up to six symbols encoded in
   mask (bit 1 = zero, bit 0 = one).  The recursion only uses addition,
   so it works for arbitrary-precision unsigned integer types
   (seq_t).  Complexity : O(n·64).  */

seq_t a(idx_t n) {
    /* There are at most 64 possible masks (0 … 63).  */
    seq_t dp[64];
    seq_t next[64];
    int i;

    /* initialise dp for length 0 (empty sequence) */
    for (i = 0; i < 64; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t len = 0; len < n; ++len) {
        /* zero out next */
        for (i = 0; i < 64; ++i) next[i] = 0;

        for (i = 0; i < 64; ++i) {
            seq_t ways = dp[i];
            if (!ways) continue;

            /* number of zeros in the stored suffix */
            int zeros = __builtin_popcount((unsigned)i);

            /* try appending a zero (bit 1) */
            if (zeros + 1 <= 2) {
                int newmask = ((i << 1) & 63) | 1;
                next[newmask] = next[newmask] + ways;
            }

            /* try appending a one (bit 0) */
            /* appending 1 never increases the zero count in the last 7 bits */
            int newmask = ((i << 1) & 63);
            next[newmask] = next[newmask] + ways;
        }

        /* swap dp and next for next iteration */
        for (i = 0; i < 64; ++i) dp[i] = next[i];
    }

    /* sum all counts for sequences of length n */
    seq_t total = 0;
    for (i = 0; i < 64; ++i) total = total + dp[i];
    return total;
}
