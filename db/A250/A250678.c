/*
 * A250678  Number of (2+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by dynamic programming over all 64 column vectors
 * of a 3×(n+1) matrix with entries 0..3.
 * The DP iteratively enforces the two monotonicity conditions:
 *   1.  for each i=1,2  :  curr[i]+prev[i] >= curr[i-1]+prev[i-1]
 *   2.  for each i=1,2  :  |curr[i]-curr[i-1]|  >=  |prev[i]-prev[i-1]|
 */
seq_t a(idx_t n)
{
    /* 3 rows, entries 0..3 → 4^3 = 64 possible column vectors.
       Encode a vector v as a base‑4 number: v = r0 + 4*r1 + 16*r2.   */
    static unsigned char row0[64], row1[64], row2[64];
    static unsigned char diff01[64], diff12[64];
    static unsigned char allowed[64][64];
    static int initialized = 0;

    if (!initialized) {
        /* pre‑compute row values and differences for all vectors */
        for (int v = 0; v < 64; ++v) {
            row0[v] = v & 3;
            row1[v] = (v >> 2) & 3;
            row2[v] = (v >> 4) & 3;
            diff01[v] = (row1[v] > row0[v]) ? (row1[v] - row0[v]) : (row0[v] - row1[v]);
            diff12[v] = (row2[v] > row1[v]) ? (row2[v] - row1[v]) : (row1[v] - row2[v]);
        }

        /* pre‑compute allowed transitions between column vectors */
        for (int p = 0; p < 64; ++p) {
            for (int c = 0; c < 64; ++c) {
                int p0 = row0[p], p1 = row1[p], p2 = row2[p];
                int c0 = row0[c], c1 = row1[c], c2 = row2[c];
                if (c1 + p1 >= c0 + p0 &&
                    c2 + p2 >= c1 + p1 &&
                    diff01[c] >= diff01[p] &&
                    diff12[c] >= diff12[p])
                    allowed[p][c] = 1;
                else
                    allowed[p][c] = 0;
            }
        }
        initialized = 1;
    }

    /* DP over the columns.  dp_cur[v] = number of ways to reach
       column vector v after processing the current column. */
    static seq_t dp_cur[64], dp_next[64];
    seq_t *cur = dp_cur, *nxt = dp_next;

    for (int v = 0; v < 64; ++v) cur[v] = 1;        /* column 0: any vector */

    for (idx_t step = 1; step <= n; ++step) {
        for (int v = 0; v < 64; ++v) nxt[v] = 0;    /* zero next DP row */

        for (int p = 0; p < 64; ++p) {
            seq_t val = cur[p];
            if (val == 0) continue;
            for (int c = 0; c < 64; ++c) {
                if (allowed[p][c]) nxt[c] += val;
            }
        }
        /* swap current and next */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum of all ways to finish at any column vector */
    seq_t result = 0;
    for (int v = 0; v < 64; ++v) result += cur[v];
    return result;
}
