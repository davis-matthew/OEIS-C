/*
 * A295249  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1 or 3 1s.
 * https://oeis.org/A295249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×4 binary arrays such that each 1 has 0,1
 *            or 3 horizontally or vertically adjacent 1s. The computation
 *            uses a DP over rows with a state consisting of the two previous
 *            rows (4-bit masks)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined, but return 0 for safety */
        return 0;

    /* ----- pre‑compute valid transitions -----
       valid[upRow][midRow][downRow] is true iff all 1s in midRow
       obtain a neighbor count (using upRow for upward neighbours and
       downRow for downward neighbours) that is 0,1 or 3. */
    static bool valid[16][16][16] = {0};
    static bool init = false;
    if (!init) {
        for (int up = 0; up < 16; ++up)
            for (int mid = 0; mid < 16; ++mid)
                for (int down = 0; down < 16; ++down) {
                    bool ok = true;
                    for (int j = 0; j < 4 && ok; ++j) {
                        if ((mid >> j) & 1) {
                            int upb   = (up >> j) & 1;
                            int left  = (j > 0)     ? ((mid >> (j - 1)) & 1) : 0;
                            int right = (j < 3)     ? ((mid >> (j + 1)) & 1) : 0;
                            int downb = (down >> j) & 1;
                            int cnt   = upb + left + right + downb;
                            if (cnt == 2 || cnt == 4)  /* disallowed */
                                ok = false;
                        }
                    }
                    valid[up][mid][down] = ok;
                }
        init = true;
    }

    /* DP arrays: dp[up][mid] holds the number of ways up to the current
       row such that the last two rows are (up,mid). */
    seq_t dp[256] = {0}, nxt[256];
    for (int mask = 0; mask < 16; ++mask)      /* first row, up row is all 0 */
        dp[mask] = 1;

    /* Process rows 2…n */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < 256; ++i) nxt[i] = 0;
        for (int up = 0; up < 16; ++up)
            for (int mid = 0; mid < 16; ++mid) {
                seq_t curCnt = dp[up * 16 + mid];
                if (curCnt == 0) continue;
                for (int down = 0; down < 16; ++down)
                    if (valid[up][mid][down]) {
                        nxt[mid * 16 + down] += curCnt;
                    }
            }
        for (int i = 0; i < 256; ++i) dp[i] = nxt[i];
    }

    /* Final check: the last row (mid) must have valid neighbour counts
       when its downward neighbour is 0. */
    seq_t result = 0;
    for (int up = 0; up < 16; ++up)
        for (int mid = 0; mid < 16; ++mid) {
            seq_t curCnt = dp[up * 16 + mid];
            if (curCnt == 0) continue;
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                if ((mid >> j) & 1) {
                    int upb   = (up >> j) & 1;
                    int left  = (j > 0)     ? ((mid >> (j - 1)) & 1) : 0;
                    int right = (j < 3)     ? ((mid >> (j + 1)) & 1) : 0;
                    int cnt   = upb + left + right;   /* down is 0 */
                    if (cnt == 2 || cnt == 4)
                        ok = false;
                }
            }
            if (ok) result += curCnt;
        }

    return result;
}
