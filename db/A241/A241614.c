/*
 * A241614  Number of length n+2 0..8 arrays with no consecutive three
 *          elements summing to more than 8.
 * https://oeis.org/A241614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes A241614: the number of length n+2 arrays with
    entries 0..8 in which no three consecutive entries have sum > 8.
    It uses dynamic programming on the last two entries of the array.
    For each pair (i,j) (0 ≤ i,j ≤ 8) we keep the count of arrays of the
    current length ending with that pair.  The array length is n+2, so we
    initialise the counts for length 2 (all 81 pairs are possible) and
    iterate up to length n+2.  Finally we sum the counts for all pairs. */

seq_t a(idx_t n)
{
    /* Length of the arrays is n+2.  For n≥1 this is at least 3. */
    idx_t target_len = n + (idx_t)2;

    /* DP arrays: dp_prev[p] = count of arrays of current length
       ending with pair (i,j) where p = i*9 + j.  Size 9*9 = 81. */
    static seq_t dp_prev[81];
    static seq_t dp_cur[81];

    /* Initialise for length 2: every pair is allowed once. */
    for (int p = 0; p < 81; ++p)
        dp_prev[p] = (seq_t)1;

    /* Transition from length 2 up to target_len. */
    for (idx_t len = (idx_t)3; len <= target_len; ++len) {
        /* Reset current DP array. */
        for (int p = 0; p < 81; ++p)
            dp_cur[p] = (seq_t)0;

        /* For each previous pair (x,y), try all possible z. */
        for (int x = 0; x <= 8; ++x) {
            for (int y = 0; y <= 8; ++y) {
                seq_t cur = dp_prev[x * 9 + y];
                if (!cur) continue;
                for (int z = 0; z <= 8; ++z) {
                    if (x + y + z <= 8) {
                        int idx = y * 9 + z;
                        dp_cur[idx] += cur;      /* arbitrary‐precision addition */
                    }
                }
            }
        }
        /* Copy for next iteration. */
        for (int p = 0; p < 81; ++p)
            dp_prev[p] = dp_cur[p];
    }

    /* Sum over all ending pairs to obtain the total count. */
    seq_t result = (seq_t)0;
    for (int p = 0; p < 81; ++p)
        result += dp_prev[p];

    return result;
}
