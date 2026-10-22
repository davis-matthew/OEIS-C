/*
 * A334300  Number of distinct nonempty subsequences (not necessarily
 *          contiguous) in the n-th composition in standard order (A066099).
 * https://oeis.org/A334300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) = number of distinct nonempty subsequences of * the n-th
 *            composition in standard order (A066099)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find positions of 1 bits in the binary expansion of n
     * (positions are counted from 0, least significant bit).          */
    idx_t temp = n;
    int pos_arr[65];                 /* maximum number of set bits ≤ 64 */
    int popcnt = 0;
    idx_t pos = 0;
    while (temp != 0) {
        if (temp & 1ULL)
            pos_arr[popcnt++] = (int)pos;
        temp >>= 1;
        ++pos;
    }

    /* Empty composition (n = 0) has no subsequences.          */
    if (popcnt == 0)
        return 0;

    /* Compute the parts of the composition.  The formula for an
     * n-th composition is:
     *   part[0] = pos_arr[0] + 1
     *   part[i] = pos_arr[i] - pos_arr[i-1]   (i ≥ 1)
     * Then reverse the array to obtain the actual composition.   */
    seq_t parts[65];
    for (int i = 0; i < popcnt; ++i) {
        if (i == 0)
            parts[i] = (seq_t)(pos_arr[0] + 1);
        else
            parts[i] = (seq_t)(pos_arr[i] - pos_arr[i-1]);
    }

    /* Count distinct subsequences using the standard DP:
     *   dp[0]   = 1  (empty subsequence)
     *   dp[i+1] = 2*dp[i] - dp[last_index]   if value has appeared before
     *   dp[i+1] = 2*dp[i]                    otherwise
     *
     *   last_index is the 0‑based position of the previous occurrence
     *   of the current value.                                     */
    const int L = popcnt;
    seq_t dp[65];
    dp[0] = 1;

    int last[65];
    for (int i = 0; i < 65; ++i)
        last[i] = -1;

    for (int i = 0; i < L; ++i) {
        /* Access composition in reverse order */
        int val = (int)parts[L-1-i];
        int prev = last[val];
        seq_t sub = (prev == -1) ? 0 : dp[prev];
        dp[i+1] = 2 * dp[i] - sub;
        last[val] = i;
    }

    /* Remove the empty subsequence */
    return dp[L] - 1;
}
