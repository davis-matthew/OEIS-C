/*
 * A268992  Number of nX5 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays in which at most one pair of
 *            horizontally or anti‑diagonally adjacent cells both contain 1.
 *            Computed by dynamic programming over the rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* horizontal edge count for each 5‑bit mask */
    static int horiz[32] = {0};
    static int horiz_inited = 0;
    if (!horiz_inited) {
        for (int mask = 0; mask < 32; ++mask) {
            int h = 0;
            for (int j = 0; j < 4; ++j)
                if ((mask >> j & 1) && (mask >> (j + 1) & 1))
                    ++h;
            horiz[mask] = h;
        }
        horiz_inited = 1;
    }

    /* dp[mask][used] : number of ways to fill rows processed so far,
       ending with current row mask = mask and `used` = 1 if an
       adjacency edge has already appeared. */
    static seq_t dp[32][2];
    static seq_t dp_next[32][2];

    for (int m = 0; m < 32; ++m) {
        dp[m][0] = 0;
        dp[m][1] = 0;
    }

    /* first row initialization */
    for (int mask = 0; mask < 32; ++mask) {
        int h = horiz[mask];
        if (h > 1) continue;
        int used = h > 0 ? 1 : 0;
        dp[mask][used] = 1;
    }

    if (n <= 1) {
        seq_t result = 0;
        for (int m = 0; m < 32; ++m) {
            result += dp[m][0];
            result += dp[m][1];
        }
        return result;
    }

    /* process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear dp_next */
        for (int m = 0; m < 32; ++m) {
            dp_next[m][0] = 0;
            dp_next[m][1] = 0;
        }

        for (int prev = 0; prev < 32; ++prev) {
            for (int used_prev = 0; used_prev <= 1; ++used_prev) {
                seq_t ways = dp[prev][used_prev];
                if (!ways) continue;
                for (int curr = 0; curr < 32; ++curr) {
                    int h_curr = horiz[curr];
                    if (h_curr > 1) continue;          /* horizontal >1 not allowed */
                    int cross = __builtin_popcount((unsigned int)(prev & (curr >> 1)));
                    int new_edges = h_curr + cross;
                    if (used_prev + new_edges > 1) continue;  /* would exceed 1 total edge */
                    int used_new = used_prev | (new_edges > 0 ? 1 : 0);
                    dp_next[curr][used_new] += ways;
                }
            }
        }

        /* copy back */
        for (int m = 0; m < 32; ++m) {
            dp[m][0] = dp_next[m][0];
            dp[m][1] = dp_next[m][1];
        }
    }

    /* sum all states */
    seq_t result = 0;
    for (int m = 0; m < 32; ++m) {
        result += dp[m][0];
        result += dp[m][1];
    }
    return result;
}
