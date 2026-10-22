/*
 * A396154  a(n) is the number of possible outcomes having a run of 5 or more
 *          sixes when a die is rolled n times.
 * https://oeis.org/A396154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A396154:
   Number of length-n sequences of die rolls (values 1–6) that contain a
   run of at least 5 consecutive sixes.
   Implementation: compute total 6^n minus the number of sequences that avoid
   any run of 5 sixes.  The avoidance count is obtained with a simple DP
   that tracks how many consecutive sixes end the current prefix. */

seq_t a(idx_t n)
{
    /* dp[k] = number of prefixes of current length ending with k consecutive sixes (0 ≤ k ≤ 4) */
    seq_t dp[5]  = {1,0,0,0,0};      /* start with empty prefix (0 sixes) */
    seq_t next[5];
    seq_t pow6 = 1;                  /* 6^current_length */

    for (idx_t i = 0; i < n; ++i)
    {
        /* reset next array */
        for (int k = 0; k < 5; ++k) next[k] = 0;

        /* transition: add a non‑six (5 choices) or a six (1 choice) */
        for (int k = 0; k < 5; ++k)
        {
            seq_t val = dp[k];
            if (val)
            {
                next[0] += val * 5;          /* adding a non‑six resets run length to 0 */
                if (k < 4)
                    next[k + 1] += val;      /* adding a six extends the run */
            }
        }

        /* copy next back to dp for next iteration */
        for (int k = 0; k < 5; ++k) dp[k] = next[k];

        /* update 6^i */
        pow6 *= 6;
    }

    /* total sequences that avoid a run of 5 sixes */
    seq_t forbidden = 0;
    for (int k = 0; k < 5; ++k) forbidden += dp[k];

    /* desired count = total sequences – forbidden sequences */
    return pow6 - forbidden;
}
