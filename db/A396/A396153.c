/*
 * A396153  a(n) is the number of possible outcomes having a run of 4 or more
 *          sixes when a die is rolled n times.
 * https://oeis.org/A396153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - number of sequences of length n over {1,…,6} that
 *            avoid a run of at least four consecutive 6’s. The value is
 *            computed via a linear recurrence on the number of ending runs of
 *            6’s up to length 3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* State counts for sequences ending with k consecutive 6's (k=0…3). */
    seq_t dp0 = (seq_t)1,   /* ending with 0 sixes */
          dp1 = (seq_t)0,   /* ending with 1 six  */
          dp2 = (seq_t)0,   /* ending with 2 sixes */
          dp3 = (seq_t)0;   /* ending with 3 sixes */

    seq_t pow6 = (seq_t)1;   /* 6^0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Sum of all states gives the number of valid sequences of length i-1. */
        seq_t total = dp0 + dp1 + dp2 + dp3;

        /* Transition for the next position. */
        seq_t newdp0 = total * (seq_t)5;   /* choose a non‑6 */
        seq_t newdp1 = dp0;                /* choose a 6 after 0 consecutive sixes */
        seq_t newdp2 = dp1;                /* choose a 6 after 1 consecutive sixes */
        seq_t newdp3 = dp2;                /* choose a 6 after 2 consecutive sixes */

        dp0 = newdp0; dp1 = newdp1; dp2 = newdp2; dp3 = newdp3;

        /* Update the total number of all sequences (6^i). */
        pow6 *= (seq_t)6;
    }

    /* Sequences avoiding four consecutive 6's. */
    seq_t avoid = dp0 + dp1 + dp2 + dp3;

    /* Required count: all sequences minus avoiding sequences. */
    return pow6 - avoid;
}
