/*
 * A297604  Number of nX5 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A297604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Transfer‑matrix DP for A297604 (nX5 0..1 arrays with the
 *            1‑neighbor rule). Initially only the pattern of the current row
 *            matters, therefore the DP keeps the last two rows (state
 *            (prev,cur)). The transition to a new row is allowed if the
 *            middle row satisfies the rule for all its 1's. All 32×32×32...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Pre‑compute the validity of every triple of rows. */
    static bool good[32][32][32];
    static unsigned char next_allowed[32][32][32];   /* list of next rows */
    static unsigned char next_count[32][32];         /* length of the list */
    static bool init = false;
    if (!init) {
        for (int above = 0; above < 32; ++above)
            for (int self = 0; self < 32; ++self)
                for (int below = 0; below < 32; ++below) {
                    bool ok = true;
                    for (int col = 0; col < 5 && ok; ++col) {
                        if ((self >> col) & 1) {
                            int cnt = 0;
                            if (col > 0 && ((self >> (col - 1)) & 1)) ++cnt;    /* left */
                            if (col < 4 && ((self >> (col + 1)) & 1)) ++cnt;    /* right */
                            if (col > 0 && ((above >> (col - 1)) & 1)) ++cnt;   /* diag up-left */
                            if (col < 4 && ((above >> (col + 1)) & 1)) ++cnt;   /* diag up-right */
                            if (col > 0 && ((below >> (col - 1)) & 1)) ++cnt;   /* diag down-left */
                            if (col < 4 && ((below >> (col + 1)) & 1)) ++cnt;   /* diag down-right */
                            if (!(cnt == 0 || cnt == 1 || cnt == 3))
                                ok = false;
                        }
                    }
                    good[above][self][below] = ok;
                }

        /* For each (prev,cur) pre‑compute admissible next rows. */
        for (int prev = 0; prev < 32; ++prev)
            for (int cur = 0; cur < 32; ++cur) {
                unsigned char idx = 0;
                for (int next = 0; next < 32; ++next)
                    if (good[prev][cur][next])
                        next_allowed[prev][cur][idx++] = (unsigned char)next;
                next_count[prev][cur] = idx;
            }
        init = true;
    }

    /* dp arrays: cur holds the current DP state,
       nxt holds the next state. */
    static seq_t dp[32][32];
    static seq_t nxt[32][32];
    seq_t (*cur)[32] = dp;
    seq_t (*ns)[32] = nxt;

    /* initialise: no previous rows, start with any first row */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            cur[i][j] = (seq_t)0;
    for (int first = 0; first < 32; ++first)
        cur[0][first] = (seq_t)1;          /* previous row is all zero */

    /* DP for rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next state */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ns[i][j] = (seq_t)0;

        /* transition */
        for (int prev = 0; prev < 32; ++prev)
            for (int cur_row = 0; cur_row < 32; ++cur_row) {
                seq_t ways = cur[prev][cur_row];
                if (ways == 0) continue;
                unsigned char lc = next_count[prev][cur_row];
                unsigned char *list = next_allowed[prev][cur_row];
                for (unsigned char k = 0; k < lc; ++k) {
                    unsigned char next_row = list[k];
                    ns[cur_row][next_row] += ways;
                }
            }
        /* swap cur and ns */
        seq_t (*tmp)[32] = cur;
        cur = ns;
        ns = tmp;
    }

    /* Final check for the last row using a dummy zero row below */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 32; ++prev)
        for (int cur_row = 0; cur_row < 32; ++cur_row)
            if (good[prev][cur_row][0])
                result += cur[prev][cur_row];

    return result;
}
