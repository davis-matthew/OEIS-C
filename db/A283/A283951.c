/*
 * A283951  Number of 2Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283951:
   Number of 2×n binary arrays with exactly one 1 that may have more than
   three of its horizontal, diagonal or antidiagonal neighbors.
   The other 1's must have at most three such neighbors.
   Implemented by a linear‑time dynamic programming that keeps the last
   two columns of the array and a flag indicating whether the unique
   violation has already occurred. */
#include <stdint.h>

static inline int bit(int pat, int row) { return (pat >> row) & 1; }

/* Count how many 1‑cells in the middle column violate the rule, given
   the left, middle and right column patterns (2‑bit values). */
static inline int viol(const int left, const int mid, const int right) {
    int v = 0;
    for (int r = 0; r < 2; ++r) {
        if (bit(mid, r)) {
            int cnt = bit(left, r)     /* left neighbour   */
                    + bit(right, r)    /* right neighbour  */
                    + bit(left, 1 - r) /* diagonal left   */
                    + bit(right, 1 - r);/* diagonal right */
            if (cnt > 3) ++v;
        }
    }
    return v;
}

/* Main function required by the build. */
seq_t a(idx_t n) {
    /* Special cases */
    if (n < 2) return (seq_t)0;   /* a(1)=0, a(0) is unused */

    /* DP arrays: dp[prev_left][prev_mid][used] */
    seq_t dp[4][4][2] = {0};
    seq_t ndp[4][4][2] = {0};

    /* Initialisation: columns 1 and 2, left sentinel is 0 */
    for (int l = 0; l < 4; ++l)
        for (int r = 0; r < 4; ++r) {
            int v = viol(0, l, r);
            if (v > 1) continue;
            int used = (v == 1 ? 1 : 0);
            dp[l][r][used] += (seq_t)1;
        }

    /* Process columns 3 … n */
    for (idx_t col = 3; col <= n; ++col) {
        /* clear ndp */
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                ndp[i][j][0] = ndp[i][j][1] = (seq_t)0;

        for (int left = 0; left < 4; ++left)
            for (int mid = 0; mid < 4; ++mid)
                for (int used = 0; used < 2; ++used) {
                    seq_t ways = dp[left][mid][used];
                    if (!ways) continue;
                    for (int cur = 0; cur < 4; ++cur) {
                        int v = viol(left, mid, cur);
                        if (v > 1) continue;
                        int used2 = used;
                        if (v == 1) {
                            if (used) continue;
                            used2 = 1;
                        }
                        ndp[mid][cur][used2] += ways;
                    }
                }

        /* copy back */
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                dp[i][j][0] = ndp[i][j][0];
                dp[i][j][1] = ndp[i][j][1];
            }
    }

    /* Final column (n) with right sentinel 0 */
    seq_t total = (seq_t)0;
    for (int left = 0; left < 4; ++left)
        for (int mid = 0; mid < 4; ++mid)
            for (int used = 0; used < 2; ++used) {
                seq_t ways = dp[left][mid][used];
                if (!ways) continue;
                int v = viol(left, mid, 0);
                if (v > 1) continue;
                int used2 = used;
                if (v == 1) {
                    if (used) continue;
                    used2 = 1;
                }
                if (used2 == 1) total += ways;
            }

    return total;
}
