/*
 * A297089  Number of nX3 0..1 arrays with no 1 adjacent to 3 king-move
 *          neighboring 1s.
 * https://oeis.org/A297089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by a simple DP over rows.
   The recurrence listed in the OEIS entry is not used directly –
   we compute all terms from the combinatorial definition that
   numbers n×3 binary arrays with no 1 having exactly 3 king‑move
   adjacent 1s.
*/

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute validity of a middle row given the rows above
       and below.  The board has width 3, so a row is a 3‑bit pattern
       in the range 0..7.  A 1 is invalid if it has exactly 3
       neighboring 1s among the surrounding cells. */
    static bool valid_middle[8][8][8];
    static bool final_valid[8][8];
    static bool init_done = false;

    if (!init_done) {
        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                for (int next = 0; next < 8; ++next) {
                    bool ok = true;
                    /* Check each 1 cell in the current row */
                    for (int j = 0; j < 3 && ok; ++j) {
                        if (!(curr & (1 << j))) continue;        /* not a 1 */
                        int neighbors = 0;
                        for (int di = -1; di <= 1; ++di) {
                            int rowBits;
                            if (di == -1)     rowBits = prev;
                            else if (di == 0) rowBits = curr;
                            else             rowBits = next;
                            for (int dj = -1; dj <= 1; ++dj) {
                                if (di == 0 && dj == 0) continue;
                                int col = j + dj;
                                if (col < 0 || col > 2) continue;
                                if (rowBits & (1 << col)) ++neighbors;
                            }
                        }
                        if (neighbors == 3) ok = false;
                    }
                    valid_middle[prev][curr][next] = ok;
                }
            }
        }
        for (int prev = 0; prev < 8; ++prev)
            for (int last = 0; last < 8; ++last)
                final_valid[prev][last] = valid_middle[prev][last][0];
        init_done = true;
    }

    /* trivial case: single row – all 8 patterns are allowed */
    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < 8; ++r)
            if (final_valid[0][r]) total += (seq_t)1;
        return total;
    }

    /* dp[prev][curr] – number of ways for the rows processed so far,
       where 'prev' is row n-1 and 'curr' is row n.  Rows above prev
       are already validated. */
    seq_t dp[8][8]   = {0};
    seq_t ndp[8][8]  = {0};

    /* Initial state after placing the first row r1 */
    for (int r1 = 0; r1 < 8; ++r1) dp[0][r1] = (seq_t)1;

    /* Add rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) ndp[i][j] = (seq_t)0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (!valid_middle[prev][curr][nxt]) continue;
                    ndp[curr][nxt] += ways;
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) dp[i][j] = ndp[i][j];
    }

    /* Final check: the last row must be valid with its above neighbor */
    seq_t result = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int last = 0; last < 8; ++last) {
            seq_t ways = dp[prev][last];
            if (ways != 0 && final_valid[prev][last]) result += ways;
        }
    }
    return result;
}
