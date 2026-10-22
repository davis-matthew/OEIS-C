/*
 * A303966  Number of nX5 0..1 arrays with every element unequal to 0, 1, 3 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary matrices with the constraint that *
 *            every cell has an unequal king‑move neighbour count of 0, 1, 3,
 *            or 5, * and the upper‑left cell is fixed to 0. * Computed via
 *            dynamic programming over rows
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;

    const int WIDTH = 5;
    const int PATS  = 1 << WIDTH;   /* 32 possible rows */
    const int NONE  = PATS;         /* index for “no row” */

    /* Pre‑compute validity of all triples (above, row, below). */
    static unsigned char valid[33][32][33];
    static int ready = 0;
    if (!ready) {
        for (int a = 0; a <= NONE; ++a)
            for (int r = 0; r < PATS; ++r)
                for (int b = 0; b <= NONE; ++b) {
                    int ok = 1;
                    for (int j = 0; j < WIDTH; ++j) {
                        int val = (r >> j) & 1;
                        int bad = 0;
                        /* same row neighbours */
                        if (j > 0)   bad += (((r >> (j-1)) & 1) != val);
                        if (j < WIDTH-1) bad += (((r >> (j+1)) & 1) != val);
                        /* neighbours from the row above */
                        if (a != NONE) {
                            bad += (((a >> j) & 1) != val);
                            if (j > 0)   bad += (((a >> (j-1)) & 1) != val);
                            if (j < WIDTH-1) bad += (((a >> (j+1)) & 1) != val);
                        }
                        /* neighbours from the row below */
                        if (b != NONE) {
                            bad += (((b >> j) & 1) != val);
                            if (j > 0)   bad += (((b >> (j-1)) & 1) != val);
                            if (j < WIDTH-1) bad += (((b >> (j+1)) & 1) != val);
                        }
                        /* allowed bad counts: 0,1,3,5 */
                        if (!(bad == 0 || bad == 1 || bad == 3 || bad == 5)) {
                            ok = 0;
                            break;
                        }
                    }
                    valid[a][r][b] = (unsigned char)ok;
                }
        ready = 1;
    }

    /* DP arrays: dp[prevprev][prev] holds counts for the current prefix. */
    seq_t dp[33][32];
    seq_t next[33][32];

    /* initialise dp */
    for (int a = 0; a <= NONE; ++a)
        for (int r = 0; r < PATS; ++r)
            dp[a][r] = 0;

    /* first row: no row above, enforce upper‑left = 0 */
    for (int r = 0; r < PATS; ++r)
        if ((r & 1) == 0)          /* column 0 is 0 */
            dp[NONE][r] = 1;

    /* process additional rows */
    for (idx_t i = 2; i <= n; ++i) {
        for (int a = 0; a <= NONE; ++a)
            for (int r = 0; r < PATS; ++r)
                next[a][r] = 0;

        for (int a = 0; a <= NONE; ++a)
            for (int r = 0; r < PATS; ++r) {
                seq_t cur = dp[a][r];
                if (cur == 0) continue;
                for (int b = 0; b < PATS; ++b) {
                    if (valid[a][r][b])
                        next[r][b] += cur;
                }
            }

        /* swap dp and next */
        for (int a = 0; a <= NONE; ++a)
            for (int r = 0; r < PATS; ++r)
                dp[a][r] = next[a][r];
    }

    /* final row validation (no row below) */
    seq_t result = 0;
    for (int a = 0; a <= NONE; ++a)
        for (int r = 0; r < PATS; ++r) {
            seq_t cur = dp[a][r];
            if (cur == 0) continue;
            if (valid[a][r][NONE])
                result += cur;
        }

    return result;
}
