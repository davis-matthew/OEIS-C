/*
 * A253346  Number of (n+1) X (4+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal maximum nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×5 0/1 arrays for which the value
 *            diagmax−antidiagmax on every 2×2 subblock is nondecreasing
 *            horizontally, vertically and from north‑east to south‑west. The
 *            array is built row by row. For each pair of consecutive rows the
 *            4‑tuple of those differences is computed and mus...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precomputed list of all 15 non‑decreasing 4‑tuples
       over the set {–1,0,1}.  The order is arbitrary but
       consistent with the mapping below. */
    static const signed char diffVec[15][4] = {
        {-1,-1,-1,-1}, {-1,-1,-1,0}, {-1,-1,-1,1},
        {-1,-1,0,0},   {-1,-1,0,1},   {-1,-1,1,1},
        {-1,0,0,0},    {-1,0,0,1},    {-1,0,1,1},
        {-1,1,1,1},
        {0,0,0,0}, {0,0,0,1}, {0,0,1,1},
        {0,1,1,1},
        {1,1,1,1}
    };

    /* Mapping from a 4‑tuple to its index in diffVec.
       Indices of the tuple entries are shifted by +1
       to fit into 0..2. */
    static int vectToIdx[3][3][3][3];
    static bool init_map = false;

    /* For each ordered pair of rows (0..31) store the
       index of the diff‑tuple produced by that pair,
       or –1 if the tuple is not monotone. */
    static int pairDiffIdx[32][32];
    static bool init_pair = false;

    if (!init_map) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    for (int l = 0; l < 3; ++l)
                        vectToIdx[i][j][k][l] = -1;

        for (int idx = 0; idx < 15; ++idx) {
            const signed char *v = diffVec[idx];
            vectToIdx[v[0] + 1][v[1] + 1][v[2] + 1][v[3] + 1] = idx;
        }
        init_map = true;
    }

    if (!init_pair) {
        for (int r0 = 0; r0 < 32; ++r0) {
            for (int r1 = 0; r1 < 32; ++r1) {
                signed char d[4];
                for (int j = 0; j < 4; ++j) {
                    unsigned char a = (r0 >> j) & 1;
                    unsigned char b = (r0 >> (j + 1)) & 1;
                    unsigned char c = (r1 >> j) & 1;
                    unsigned char d_ = (r1 >> (j + 1)) & 1;
                    int diag = (a > d_) ? a : d_;
                    int anti = (b > c) ? b : c;
                    d[j] = diag - anti;               /* ∈ {–1,0,1} */
                }
                if (d[0] <= d[1] && d[1] <= d[2] && d[2] <= d[3]) {
                    int idx = vectToIdx[d[0] + 1][d[1] + 1][d[2] + 1][d[3] + 1];
                    pairDiffIdx[r0][r1] = idx;         /* list of indices 0..14 */
                } else {
                    pairDiffIdx[r0][r1] = -1;          /* forbidden pair */
                }
            }
        }
        init_pair = true;
    }

    /* Dynamic programming tables.
       dp[row][idx] = number of arrays whose last row is 'row'
       and whose last diff‑tuple index is 'idx'. */
    seq_t dp[32][15] = {0};
    seq_t newdp[32][15] = {0};

    /* Base case: n = 1 => 2 rows, one diff‑row. */
    for (int r0 = 0; r0 < 32; ++r0) {
        for (int r1 = 0; r1 < 32; ++r1) {
            int idx = pairDiffIdx[r0][r1];
            if (idx != -1) {
                dp[r1][idx] += (seq_t)1;
            }
        }
    }

    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < 32; ++r)
            for (int i = 0; i < 15; ++i)
                total += dp[r][i];
        return total;
    }

    /* For additional rows, iterate from 2 to n. */
    for (idx_t step = 2; step <= n; ++step) {
        /* reset newdp */
        for (int r = 0; r < 32; ++r)
            for (int i = 0; i < 15; ++i)
                newdp[r][i] = 0;

        for (int prevRow = 0; prevRow < 32; ++prevRow) {
            for (int prevIdx = 0; prevIdx < 15; ++prevIdx) {
                seq_t count = dp[prevRow][prevIdx];
                if (!count) continue;
                const signed char *dPrev = diffVec[prevIdx];
                for (int nextRow = 0; nextRow < 32; ++nextRow) {
                    int currIdx = pairDiffIdx[prevRow][nextRow];
                    if (currIdx == -1) continue;
                    const signed char *dCurr = diffVec[currIdx];

                    /* vertical monotonicity: dPrev[j] <= dCurr[j] */
                    if (dPrev[0] > dCurr[0] || dPrev[1] > dCurr[1] ||
                        dPrev[2] > dCurr[2] || dPrev[3] > dCurr[3])
                        continue;

                    /* anti‑diagonal monotonicity: dPrev[j+1] <= dCurr[j] */
                    if (dPrev[1] > dCurr[0] || dPrev[2] > dCurr[1] ||
                        dPrev[3] > dCurr[2])
                        continue;

                    newdp[nextRow][currIdx] += count;
                }
            }
        }

        /* copy newdp to dp */
        for (int r = 0; r < 32; ++r)
            for (int i = 0; i < 15; ++i)
                dp[r][i] = newdp[r][i];
    }

    /* Sum all remaining states to get a(n) */
    seq_t result = 0;
    for (int r = 0; r < 32; ++r)
        for (int i = 0; i < 15; ++i)
            result += dp[r][i];
    return result;
}
