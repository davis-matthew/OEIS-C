/*
 * A268453  Number of length-n 0..4 arrays with no adjacent pair x,x+1
 *          followed at any distance by x+1,x.
 * https://oeis.org/A268453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268453: number of length‑n 0..4 arrays with no adjacent pair
   x,x+1 followed anywhere later by x+1,x.  Computed by dynamic programming
   over positions and a 4‑bit mask recording which upward pairs
   (i,i+1) have already appeared.  This ensures that a later
   downward pair (i+1,i) is never allowed if the corresponding upward
   pair has occurred.  The DP uses 5*16=80 states and runs in O(n) time. */
seq_t a(idx_t n)
{
    /* dp[prev][mask] – count of arrays of current length ending with
       value 'prev' and having upward pairs encoded in 'mask' (bit i set
       means the pair (i,i+1) has appeared).  */
    seq_t dp[5][16];
    seq_t ndp[5][16];
    int i, m, prev, next, mask, newMask;

    /* initialise for length 1 */
    for (i = 0; i < 5; ++i)
        for (m = 0; m < 16; ++m)
            dp[i][m] = 0;
    for (i = 0; i < 5; ++i)
        dp[i][0] = 1;

    /* extend the arrays to length n */
    for (idx_t pos = 1; pos < n; ++pos) {
        /* clear next state */
        for (i = 0; i < 5; ++i)
            for (m = 0; m < 16; ++m)
                ndp[i][m] = 0;

        for (prev = 0; prev < 5; ++prev)
            for (mask = 0; mask < 16; ++mask) {
                seq_t cur = dp[prev][mask];
                if (cur == 0) continue;

                for (next = 0; next < 5; ++next) {
                    if (next == prev) {
                        ndp[next][mask] += cur;
                    } else if (next == prev + 1) {          /* upward pair */
                        newMask = mask | (1 << prev);
                        ndp[next][newMask] += cur;
                    } else if (next == prev - 1) {          /* downward pair */
                        if (mask & (1 << (prev - 1))) continue; /* forbidden */
                        ndp[next][mask] += cur;
                    } else {                                /* other neighbour */
                        ndp[next][mask] += cur;
                    }
                }
            }

        /* copy ndp back to dp for next iteration */
        for (i = 0; i < 5; ++i)
            for (m = 0; m < 16; ++m)
                dp[i][m] = ndp[i][m];
    }

    /* sum all final states */
    seq_t result = 0;
    for (i = 0; i < 5; ++i)
        for (m = 0; m < 16; ++m)
            result += dp[i][m];

    return result;
}
