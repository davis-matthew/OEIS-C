/*
 * A384616  A(m,n) is the maximum sum of absolute differences of the labels of
 *          adjacent vertices of the grid graph P_m X P_n where the m*n labels
 *          are exactly 1, 2, ..., m*n.
 * https://oeis.org/A384616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) = maximum sum of absolute differences of labels of adjacent
 *            vertices on the grid graph P_m × P_n, where m*n = m*n and the
 *            labels are 1..m*n. The index n maps to the pair (m,n) by the
 *            triangular enumeration: n = 1,2,3,4,5,6,… corresponds to
 *            (1,1),(2,1),(2,2),(3,1),(3,2),(3,3),… This implement...
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   sum_range
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_range.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds, but safe */

    /* Find smallest m with m*(m+1)/2 >= n */
    idx_t m = 1;
    while (((m * (m + 1)) / 2) < n) ++m;

    /* The starting index of this row */
    idx_t start = ((m - 1) * m) / 2 + 1;
    idx_t col   = n - start + 1;          /* column index (1 … m) */

    /* Count vertices of each degree for both bipartite parts */
    idx_t cntA[5] = {0, 0, 0, 0, 0};
    idx_t cntB[5] = {0, 0, 0, 0, 0};

    for (idx_t i = 1; i <= m; ++i)
        for (idx_t j = 1; j <= col; ++j)
        {
            idx_t deg = 0;
            if (i > 1) ++deg;
            if (i < m) ++deg;
            if (j > 1) ++deg;
            if (j < col) ++deg;

            int part = (int)((i + j) & 1);   /* 0 → part A, 1 → part B */
            if (part == 0)
                ++cntA[(int)deg];
            else
                ++cntB[(int)deg];
        }

    /* Weighted sums of labels for the two parts */
    seq_t totalA = 0;     /* part A contributes negatively */
    seq_t totalB = 0;     /* part B contributes positively */

    idx_t label = 1;       /* next label to assign */

    /* Assign labels to part A: degrees in descending order */
    for (int d = 4; d >= 0; --d)
    {
        idx_t c = cntA[d];
        if (c == 0) continue;
        idx_t s = label;
        idx_t e = label + c - 1;
        seq_t ws = sum_range(s, e);
        totalA += (seq_t)d * ws;
        label += c;
    }

    /* Assign labels to part B: degrees in ascending order */
    for (int d = 0; d <= 4; ++d)
    {
        idx_t c = cntB[d];
        if (c == 0) continue;
        idx_t s = label;
        idx_t e = label + c - 1;
        seq_t ws = sum_range(s, e);
        totalB += (seq_t)d * ws;
        label += c;
    }

    /* Final answer: sum over B minus sum over A */
    return totalB - totalA;
}
