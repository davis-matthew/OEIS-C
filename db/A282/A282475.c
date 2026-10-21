/*
 * A282475  Number of n X 2 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors, with the exception of exactly two
 *          elements.
 * https://oeis.org/A282475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A282475 – number of n×2 binary matrices with exactly two violations
 *  (a 1 having more than three king-move neighbours).  The value is
 *  obtained by dynamic programming over rows, counting violations as
 *  each row becomes a middle row of a consecutive triple.  A final
 *  dummy zero row is added after the last real row to count violations
 *  for the last row.  The algorithm runs in O(n) time with a
 *  constant amount of state.  */
seq_t a(idx_t n) {
    /* Pre‑compute the number of violations caused by a middle row p
       given previous row pp and next row nrow.  The maximum number
       of violations a row can introduce is 2, so an int is sufficient
       for storage. */
    static int viol[4][4][4] = {0};
    static int viol_inited = 0;
    if (!viol_inited) {
        for (int pp = 0; pp < 4; ++pp) {
            for (int p  = 0; p  < 4; ++p) {
                for (int nr = 0; nr < 4; ++nr) {
                    int v = 0;
                    int ap0 = (p>>0)&1, ap1 = (p>>1)&1;
                    int bp0 = (pp>>0)&1, bp1 = (pp>>1)&1;
                    int cp0 = (nr>>0)&1, cp1 = (nr>>1)&1;
                    /* cell (row, col 0) */
                    int neigh0 = ap1 + bp0 + bp1 + cp0 + cp1;
                    if (ap0 && neigh0 > 3) ++v;
                    /* cell (row, col 1) */
                    int neigh1 = ap0 + bp1 + bp0 + cp1 + cp0;
                    if (ap1 && neigh1 > 3) ++v;
                    viol[pp][p][nr] = v;
                }
            }
        }
        viol_inited = 1;
    }

    /* dp[state][k] – number of ways to form the processed prefix
       where state = prev_row*4 + cur_row (0…15) and k is the
       number of violations so far (0…2). */
    static seq_t dp[16][3];
    static seq_t next[16][3];

    /* initialise for the first row: previous row is all‑zeros. */
    for (int i = 0; i < 16; ++i)
        for (int k = 0; k < 3; ++k)
            dp[i][k] = 0;
    for (int cur = 0; cur < 4; ++cur)
        dp[0*4 + cur][0] = dp[0*4 + cur][0] + 1;

    /* process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 16; ++i)
            for (int k = 0; k < 3; ++k)
                next[i][k] = 0;
        for (int pp = 0; pp < 4; ++pp) {
            for (int p = 0; p < 4; ++p) {
                int idx = pp*4 + p;
                for (int k = 0; k < 3; ++k) {
                    seq_t val = dp[idx][k];
                    if (!val) continue;
                    for (int nr = 0; nr < 4; ++nr) {
                        int add = viol[pp][p][nr];   /* int */
                        int nk  = k + add;
                        if (nk > 2) continue;
                        int next_idx = p*4 + nr;
                        next[next_idx][nk] = next[next_idx][nk] + val;
                    }
                }
            }
        }
        for (int i = 0; i < 16; ++i)
            for (int k = 0; k < 3; ++k)
                dp[i][k] = next[i][k];
    }

    /* final dummy zero row – count violations of the last real row. */
    seq_t result = 0;
    for (int prev = 0; prev < 4; ++prev) {
        for (int last = 0; last < 4; ++last) {
            int idx = prev*4 + last;
            for (int k = 0; k < 3; ++k) {
                seq_t val = dp[idx][k];
                if (!val) continue;
                int nk = k + viol[prev][last][0];
                if (nk == 2)
                    result = result + val;
            }
        }
    }
    return result;
}
