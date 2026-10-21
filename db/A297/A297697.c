/*
 * A297697  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A297697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n 0..1 arrays in which every 1 has a
 *            horizontal, diagonal or anti‑diagonal neighbour count belonging
 *            to {0, 2, 4}. The adjacency considered for each cell is: -
 *            left/right on the same row, - up‑left, up‑right, down‑left,
 *            down‑right. Vertical neighbours are not counted
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Pre‑compute validity of three consecutive columns.
   LCR[L][C][R] is true iff column C satisfies the neighbour rule
   when its left neighbour is L and its right neighbour is R.  */
static bool LCR[16][16][16];
static bool LCR_precomputed = false;

static void compute_LCR(void)
{
    if (LCR_precomputed) return;
    LCR_precomputed = true;

    for (int L = 0; L < 16; ++L) {
        for (int C = 0; C < 16; ++C) {
            for (int R = 0; R < 16; ++R) {
                bool ok = true;
                /* examine each row of the middle column C */
                for (int r = 0; r < 4; ++r) {
                    if ((C >> r) & 1) {
                        int cnt = 0;
                        /* horizontal neighbours */
                        if ((L >> r) & 1) ++cnt;
                        if ((R >> r) & 1) ++cnt;
                        /* diagonal neighbours */
                        if (r < 3 && (L >> (r + 1)) & 1) ++cnt;  /* up‑left  */
                        if (r > 0 && (L >> (r - 1)) & 1) ++cnt;  /* down‑left */
                        if (r < 3 && (R >> (r + 1)) & 1) ++cnt;  /* up‑right */
                        if (r > 0 && (R >> (r - 1)) & 1) ++cnt;  /* down‑right*/
                        if (cnt != 0 && cnt != 2 && cnt != 4) {
                            ok = false;
                            break;
                        }
                    }
                }
                LCR[L][C][R] = ok;
            }
        }
    }
}

extern seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */

    compute_LCR();

    static seq_t dp[2][16][16];
    int cur = 0, nxt = 1;

    /* initialise DP: before any column there is a dummy left column 0.
       All possible first columns are admissible but not yet validated. */
    for (int p = 0; p < 16; ++p)
        for (int c = 0; c < 16; ++c)
            dp[cur][p][c] = 0;
    for (int c = 0; c < 16; ++c)
        dp[cur][0][c] = 1;

    /* add columns one by one (n‑1 transitions) */
    for (idx_t step = 1; step < n; ++step) {
        /* zero next layer */
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                dp[nxt][p][c] = 0;

        /* transition: validate current column with left=p and right=nn */
        for (int p = 0; p < 16; ++p) {
            for (int c = 0; c < 16; ++c) {
                seq_t val = dp[cur][p][c];
                if (!val) continue;
                for (int nn = 0; nn < 16; ++nn) {
                    if (!LCR[p][c][nn]) continue;
                    dp[nxt][c][nn] += val;
                }
            }
        }
        /* swap layers */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* final validation: rightmost column must see a right neighbour 0 */
    seq_t ans = 0;
    for (int p = 0; p < 16; ++p) {
        for (int c = 0; c < 16; ++c) {
            if (dp[cur][p][c] && LCR[p][c][0]) ans += dp[cur][p][c];
        }
    }
    return ans;
}
