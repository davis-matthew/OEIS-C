/*
 * A321251  a(n) is the number of ways to place non-attacking knights on a 3 X
 *          n chessboard.
 * https://oeis.org/A321251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A321251: number of ways to place non‑attacking knights on a 3×n chessboard.
 *  The sequence satisfies the transfer‑matrix recurrence described in the code.
 *  For each column we keep the occupancy of the two previous columns.
 *  The implementation runs in O(n) time and uses only 64-bit masks
 *  for intermediate states.  The function is pure and thread‑safe. */
 
seq_t a(idx_t n)
{
    /* DP state: 6 bits = (prev2 << 3) | prev1, each 3 bits for a column. */
    seq_t dp[64] = {0};
    seq_t dp_next[64] = {0};
    int i, cur_bits, prev1, prev2, new_mask;
    seq_t result = 0;

    /* Special case: n = 0 → empty board. */
    if (n == 0) return 1;

    /* Initial state: both previous columns empty. */
    dp[0] = 1;

    for (idx_t col = 0; col < n; ++col) {
        /* clear next dp array */
        for (i = 0; i < 64; ++i) dp_next[i] = 0;

        for (i = 0; i < 64; ++i) {
            if (dp[i] == 0) continue;
            prev2 = i >> 3;
            prev1 = i & 7;

            /* try all 8 possible bit patterns for the new column */
            for (cur_bits = 0; cur_bits < 8; ++cur_bits) {
                /* check conflicts with prev1 (col-1): rows 0↔2 */
                if ((prev1 & 1) && (cur_bits & 4)) continue;
                if ((prev1 & 4) && (cur_bits & 1)) continue;

                /* check conflicts with prev2 (col-2): row diff 1 */
                if ((prev2 & 1) && (cur_bits & 2)) continue;
                if ((prev2 & 2) && (cur_bits & 1)) continue;
                if ((prev2 & 2) && (cur_bits & 4)) continue;
                if ((prev2 & 4) && (cur_bits & 2)) continue;

                new_mask = (prev1 << 3) | cur_bits;
                dp_next[new_mask] += dp[i];
            }
        }
        /* swap dp and dp_next */
        for (i = 0; i < 64; ++i) dp[i] = dp_next[i];
    }

    /* Sum over all ending states */
    for (i = 0; i < 64; ++i)
        result += dp[i];

    return result;
}
