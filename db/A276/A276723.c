/*
 * A276723  Number of set partitions of [n] such that for each block b the
 *          smallest integer interval containing b has at most seven elements.
 * https://oeis.org/A276723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of set partitions of [n] such that for each block
 *            b the smallest integer interval containing b has at most seven
 *            elements. Computation via dynamic programming over a 7‑bit mask
 *            representing the starts of blocks that are still open (the
 *            distance from the current index is at most 6)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const int MASK_LIMIT = 128;          /* 2^7 states for open block starts */
    const int FULL_MASK  = 0x7F;          /* 0b1111111 */

    seq_t dp[MASK_LIMIT];
    seq_t dp_next[MASK_LIMIT];

    /* initialise the DP: before processing any element there are no open blocks */
    for (int m = 0; m < MASK_LIMIT; ++m) dp[m] = 0;
    dp[0] = 1;

    /* process each element 1..n */
    for (idx_t pos = 1; pos <= n; ++pos) {
        /* reset next array */
        for (int m = 0; m < MASK_LIMIT; ++m) dp_next[m] = 0;

        /* transition for each current state */
        for (int m = 0; m < MASK_LIMIT; ++m) {
            seq_t cur = dp[m];
            if (cur == 0) continue;

            int open = __builtin_popcount((unsigned)m);   /* # of open blocks */

            /* option 1: assign current element to one of the open blocks */
            int shift = (m << 1) & FULL_MASK;
            dp_next[shift] += cur * (seq_t)open;

            /* option 2: start a new block at current position */
            int new_mask = (shift | 0x02) & FULL_MASK;   /* bit 1 corresponds to start at this position */
            dp_next[new_mask] += cur;
        }

        /* prepare for next position */
        for (int m = 0; m < MASK_LIMIT; ++m) dp[m] = dp_next[m];
    }

    /* after all elements, any remaining open blocks are already closed;
       sum all DP states to obtain the final count. */
    seq_t total = 0;
    for (int m = 0; m < MASK_LIMIT; ++m) total += dp[m];
    return total;
}
