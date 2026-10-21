/*
 * A268736  Number of nX4 binary arrays with some 1 horizontally or vertically
 *          adjacent to some other 1 exactly once.
 * https://oeis.org/A268736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268736: number of n×4 binary arrays with exactly one
 * horizontally or vertically adjacent pair of 1s.
 * The function uses a dynamic–programming recurrence over the rows.
 */
seq_t a(idx_t n)
{
    /* Number of horizontal adjacencies within each 4‑bit mask. */
    static const int horiz_adj[16] = {
        0, 0, 0, 1,  /* 0000, 0001, 0010, 0011 */
        0, 0, 1, 2,  /* 0100, 0101, 0110, 0111 */
        0, 0, 0, 1,  /* 1000, 1001, 1010, 1011 */
        1, 1, 2, 3   /* 1100, 1101, 1110, 1111 */
    };

    static seq_t dp_prev[16][2];
    static seq_t dp_cur[16][2];

    /* Initialise dp_prev for the first row. */
    for (int m = 0; m < 16; ++m) {
        dp_prev[m][0] = 0;
        dp_prev[m][1] = 0;
    }
    for (int m = 0; m < 16; ++m) {
        int h = horiz_adj[m];
        if (h <= 1)
            dp_prev[m][h] = 1;
    }

    /* Process the remaining rows (if any). */
    for (idx_t row = 2; row <= n; ++row) {
        /* Reset current DP table. */
        for (int m = 0; m < 16; ++m) {
            dp_cur[m][0] = 0;
            dp_cur[m][1] = 0;
        }

        for (int prev = 0; prev < 16; ++prev) {
            for (int k = 0; k <= 1; ++k) {
                seq_t val = dp_prev[prev][k];
                if (val == 0) continue;

                for (int cur = 0; cur < 16; ++cur) {
                    int h = horiz_adj[cur];
                    int v = __builtin_popcount((unsigned int)(prev & cur));
                    int newk = k + h + v;
                    if (newk > 1) continue;
                    dp_cur[cur][newk] += val;
                }
            }
        }

        /* Copy dp_cur back to dp_prev for the next iteration. */
        for (int m = 0; m < 16; ++m) {
            dp_prev[m][0] = dp_cur[m][0];
            dp_prev[m][1] = dp_cur[m][1];
        }
    }

    /* Sum all configurations with exactly one adjacent pair. */
    seq_t result = 0;
    for (int m = 0; m < 16; ++m)
        result += dp_prev[m][1];

    return result;
}
