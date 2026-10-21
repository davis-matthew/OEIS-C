/*
 * A283199  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          horizontal and vertical neighbors, with the exception of exactly
 *          one element.
 * https://oeis.org/A283199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A283199 (column 4 of A283203):
   Number of n×4 binary arrays with no 1 having more than two
   horizontal or vertical 1‑neighbours,
   except that exactly one element may violate this rule.
   The algorithm is a dynamic programming over rows:
   state = (row i‑2, row i‑1, violations seen (0 or 1)).
   When a new row is appended, the violations in the middle row
   (row i‑1) are evaluated using its two neighbours, and the
   state is advanced.  After processing a dummy bottom row,
   the number of arrays with exactly one violation is accumulated.
   Complexity: O( n·512·16 ), well below the limit for n≈10⁴.  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the number of 1‑neighbour violations in
       a middle row given the patterns of the rows above (p2),
       below (p0) and in the row itself (p1). */
    static unsigned char viol[16][16][16];
    static bool init = false;
    if (!init) {
        for (int p2 = 0; p2 < 16; ++p2)
            for (int p1 = 0; p1 < 16; ++p1)
                for (int p0 = 0; p0 < 16; ++p0) {
                    int cnt = 0;
                    for (int j = 0; j < 4; ++j) {
                        if ((p1 >> j) & 1U) {
                            int neigh = 0;
                            if (j > 0)      neigh += (p1 >> (j-1)) & 1U;
                            if (j < 3)      neigh += (p1 >> (j+1)) & 1U;
                            neigh += (p2 >> j) & 1U;   // above
                            neigh += (p0 >> j) & 1U;   // below
                            if (neigh > 2) ++cnt;      // violation
                        }
                    }
                    viol[p2][p1][p0] = (unsigned char)cnt;
                }
        init = true;
    }

    /* DP tables: dp_arr[p2][p1][v] = number of ways after processing
       the current number of rows, with last two rows p2 (row i‑2) and
       p1 (row i‑1) and v violations counted so far (v = 0 or 1). */
    static seq_t dp_arr[16][16][2];
    static seq_t ndp_arr[16][16][2];
    seq_t (*cur)[16][2] = dp_arr;
    seq_t (*nxt)[16][2] = ndp_arr;

    /* initialise current DP to zero and set start state */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            cur[i][j][0] = cur[i][j][1] = 0;
    cur[0][0][0] = (seq_t)1;        /* nothing processed yet, no violations */

    /* main DP loop over all rows plus a dummy bottom row */
    for (idx_t step = 0; step <= n; ++step) {
        /* zero next DP table */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                nxt[i][j][0] = nxt[i][j][1] = 0;

        int cur_low  = 0;
        int cur_high = (step == n) ? 1 : 16;   /* 0 for dummy, else all rows */

        for (int p2 = 0; p2 < 16; ++p2)
            for (int p1 = 0; p1 < 16; ++p1)
                for (int v = 0; v < 2; ++v) {
                    seq_t val = cur[p2][p1][v];
                    if (val == 0) continue;

                    for (int p0 = cur_low; p0 < cur_high; ++p0) {
                        int vadd = viol[p2][p1][p0];
                        int nv   = v + vadd;
                        if (nv > 1) continue;           /* at most one violation */
                        nxt[p1][p0][nv] += val;
                    }
                }

        /* swap current and next tables for next iteration */
        seq_t (*tmp)[16][2] = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* answer: sum over states with exactly one violation */
    seq_t ans = 0;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            ans += cur[i][j][1];
    return ans;
}
