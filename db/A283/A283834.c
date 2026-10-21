/*
 * A283834  Number of length-n binary vectors beginning with 0, ending with 1,
 *          and avoiding 4 consecutive 0's and 4 consecutive 1's.
 * https://oeis.org/A283834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that begin with 0,
 *            end with 1, and contain no run of four identical bits.
 *            Computation is done by a linear DP over the last bit and current
 *            run length. The recurrence has constant memory and is O(n) time
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 74, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* by definition of the sequence */
    if (n == 1) return (seq_t)0;   /* no string of length 1 can start 0 and end 1 */

    /* dp[last][len] : number of strings of current length ending with
       bit 'last' (0 or 1) and having a run of 'len' (1..3).  Only
       len 1..3 are used; index 0 is unused. */
    seq_t dp_curr[2][4] = {{0}};
    seq_t dp_next[2][4];

    dp_curr[0][1] = (seq_t)1;  /* string of length 1: "0" */

    for (idx_t pos = 1; pos < n; ++pos) {
        /* zero dp_next */
        for (int last = 0; last < 2; ++last)
            for (int len = 1; len <= 3; ++len)
                dp_next[last][len] = (seq_t)0;

        for (int last = 0; last < 2; ++last) {
            for (int len = 1; len <= 3; ++len) {
                seq_t val = dp_curr[last][len];
                if (val == 0) continue;
                /* append same bit, if run can be extended */
                if (len < 3)
                    dp_next[last][len+1] += val;
                /* append opposite bit */
                dp_next[1-last][1] += val;
            }
        }
        /* move dp_next to dp_curr for next iteration */
        for (int last = 0; last < 2; ++last)
            for (int len = 1; len <= 3; ++len)
                dp_curr[last][len] = dp_next[last][len];
    }

    /* sum all ways that end with a 1 (last bit 1) */
    seq_t result = (seq_t)0;
    for (int len = 1; len <= 3; ++len)
        result += dp_curr[1][len];
    return result;
}
