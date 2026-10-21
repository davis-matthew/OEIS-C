/*
 * A282833  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors, with the exception of exactly two elements.
 * https://oeis.org/A282833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A282833
 *  a(n) = number of n×3 binary arrays in which no 1 has more than one
 *  king‑move neighbour, except that exactly two cells may violate
 *  this rule.
 *
 *  The sequence does not have a simple closed form.  The implementation
 *  below uses dynamic programming over rows: it keeps the last two rows
 *  (as 3‑bit masks) and the current number of violating cells (0,1,2).
 *  For each new row we update the count of violations of the current row
 *  using a pre‑computed table of violations for a given pattern of
 *  (previous row, current row, next row).  Only two‐row memory is used,
 *  so the time is O(n) and the space is O(1) (apart from a few small
 *  tables).  The procedure works for the arbitrary‑precision type
 *  seq_t supplied by the build system and is valid both in C11 and
 *  C++17.  All operations use only +, -, *, /, %, <, >, <=, >=,
 *  &&, ||, !, bitwise operators and simple assignments, as required.
 */

#include <stdbool.h>
#include <string.h>

static unsigned char viol_cache[8][8][8];
static bool viol_inited = false;

/* pre‑compute the number of cells in ‘cur’ that have >1 neighbour
 * when the neighbour rows are ‘above’ and ‘below’. */
static void init_viol_cache(void)
{
    if (viol_inited) return;
    for (int above = 0; above < 8; ++above) {
        for (int cur = 0; cur < 8; ++cur) {
            for (int below = 0; below < 8; ++below) {
                int cnt = 0;
                for (int c = 0; c < 3; ++c) {
                    if (!(cur & (1 << c))) continue;
                    int neigh = 0;
                    for (int dr = -1; dr <= 1; ++dr) {
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dr == 0 && dc == 0) continue;
                            int nc = c + dc;
                            if (nc < 0 || nc > 2) continue;
                            unsigned char mask = (dr == -1) ? above
                                            : (dr == 0) ? cur
                                            : below;
                            if (mask & (1 << nc)) ++neigh;
                        }
                    }
                    if (neigh > 1) ++cnt;
                }
                viol_cache[above][cur][below] = (unsigned char)cnt;
            }
        }
    }
    viol_inited = true;
}

seq_t a(idx_t n)
{
    /* handle indices below offset (index 0) */
    if (n == 0) return (seq_t)0;

    init_viol_cache();

    /* dp[prev][cur][k]  – ways for processed rows so far,
     * previous row = prev, current row = cur, number of violating
     * cells seen so far = k (0..2).  Only cells up to and
     * including the current row have been evaluated.  k never exceeds 2
     * because states with more than two violations are discarded. */
    seq_t dp[8][8][3];
    seq_t dp_next[8][8][3];

    /* zero the tables */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k] = (seq_t)0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 3; ++k)
                dp_next[i][j][k] = (seq_t)0;

    /* initialise with first row (row index 1).  Above row is all 0. */
    for (int cur = 0; cur < 8; ++cur) {
        dp[0][cur][0] = (seq_t)1;
    }

    /* process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        /* reset dp_next to zero */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    dp_next[i][j][k] = (seq_t)0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int cur = 0; cur < 8; ++cur) {
                for (int k = 0; k <= 2; ++k) {
                    seq_t ways = dp[prev][cur][k];
                    if (ways == (seq_t)0) continue;
                    for (int next = 0; next < 8; ++next) {
                        int v = viol_cache[prev][cur][next];
                        int nk = k + v;
                        if (nk > 2) continue;
                        dp_next[cur][next][nk] = dp_next[cur][next][nk] + ways;
                    }
                }
            }
        }

        /* copy dp_next into dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = dp_next[i][j][k];
    }

    /* after all rows processed, we must account for violations of
     * the last row (cur) using a fictitious below row of 0. */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int cur = 0; cur < 8; ++cur) {
            for (int k = 0; k <= 2; ++k) {
                seq_t ways = dp[prev][cur][k];
                if (ways == (seq_t)0) continue;
                int v = viol_cache[prev][cur][0];
                int nk = k + v;
                if (nk == 2) ans = ans + ways;
            }
        }
    }

    return ans;
}
