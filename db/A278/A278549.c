/*
 * A278549  Number of nX3 0..3 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly two mistakes.
 * https://oeis.org/A278549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 0..3 arrays with rows and columns in
 *            lexicographic * nondecreasing order, but with exactly two
 *            mistakes (row or column order violations). * Computation uses
 *            dynamic programming over rows, tracking the state of column
 *            order * (for the two adjacent column pairs) and the number...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Precomputation tables */
static int digits[64][3];
static unsigned char pairState[64][2];
static unsigned char trans[64][9];
static bool precomp_done = false;

/* Initialize the tables once */
static void init_precomp_func(void)
{
    /* digit extraction: most significant digit is column 0 */
    for (int r = 0; r < 64; ++r)
    {
        digits[r][0] = (r >> 4) & 3;   /* column 0 */
        digits[r][1] = (r >> 2) & 3;   /* column 1 */
        digits[r][2] = r & 3;          /* column 2 */
    }

    /* state describing comparison of a row's adjacent columns */
    for (int r = 0; r < 64; ++r)
    {
        pairState[r][0] = (digits[r][0] < digits[r][1]) ? 1 :
                          (digits[r][0] > digits[r][1]) ? 2 : 0;
        pairState[r][1] = (digits[r][1] < digits[r][2]) ? 1 :
                          (digits[r][1] > digits[r][2]) ? 2 : 0;
    }

    /* transition table: given a previous column state and a new row,
     * produce the new column state after processing that row */
    for (int r = 0; r < 64; ++r)
    {
        for (int s = 0; s < 9; ++s)
        {
            int prev0 = s / 3;
            int prev1 = s % 3;
            int ns0 = (prev0 == 0) ? pairState[r][0] : prev0;
            int ns1 = (prev1 == 0) ? pairState[r][1] : prev1;
            trans[r][s] = (unsigned char)(ns0 * 3 + ns1);
        }
    }

    precomp_done = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!precomp_done) init_precomp_func();

    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* DP tables: dp[rowViol][lastRow][colState] */
    static seq_t dp_cur[3][64][9];
    static seq_t dp_next[3][64][9];

    /* zero initialize dp_cur */
    for (int v = 0; v < 3; ++v)
        for (int r = 0; r < 64; ++r)
            for (int s = 0; s < 9; ++s)
                dp_cur[v][r][s] = (seq_t)0;

    /* first row (no row comparison yet) */
    for (int r = 0; r < 64; ++r)
    {
        int state = pairState[r][0] * 3 + pairState[r][1];
        dp_cur[0][r][state] = dp_cur[0][r][state] + (seq_t)1;
    }

    /* process subsequent rows */
    for (idx_t rowIdx = 2; rowIdx <= n; ++rowIdx)
    {
        /* zero next table */
        for (int v = 0; v < 3; ++v)
            for (int r = 0; r < 64; ++r)
                for (int s = 0; s < 9; ++s)
                    dp_next[v][r][s] = (seq_t)0;

        /* cumulative sums for all rows (lexicographic order = numeric order) */
        static seq_t total[3][9];
        static seq_t prefix[3][9][65];
        for (int v = 0; v < 3; ++v)
        {
            for (int s = 0; s < 9; ++s)
            {
                seq_t sum = 0;
                prefix[v][s][0] = sum;
                for (int r = 0; r < 64; ++r)
                {
                    sum += dp_cur[v][r][s];
                    prefix[v][s][r + 1] = sum;
                }
                total[v][s] = sum;
            }
        }

        /* transitions */
        for (int r2 = 0; r2 < 64; ++r2)
        {
            int rank2 = r2;        /* lexicographic rank = numeric value */
            for (int v = 0; v < 3; ++v)
            {
                for (int s = 0; s < 9; ++s)
                {
                    seq_t sum_inc0 = prefix[v][s][rank2 + 1];
                    seq_t sum_inc1 = total[v][s] - sum_inc0;
                    int newState   = trans[r2][s];

                    if (sum_inc0 != (seq_t)0)
                        dp_next[v][r2][newState] = dp_next[v][r2][newState] + sum_inc0;
                    if ((v + 1) < 3 && sum_inc1 != (seq_t)0)
                        dp_next[v + 1][r2][newState] = dp_next[v + 1][r2][newState] + sum_inc1;
                }
            }
        }

        /* copy next to current for next iteration */
        for (int v = 0; v < 3; ++v)
            for (int r = 0; r < 64; ++r)
                for (int s = 0; s < 9; ++s)
                    dp_cur[v][r][s] = dp_next[v][r][s];
    }

    /* final aggregation: count arrays with exactly two total violations */
    seq_t ans = 0;
    for (int v = 0; v < 3; ++v)
    {
        for (int r = 0; r < 64; ++r)
        {
            for (int s = 0; s < 9; ++s)
            {
                seq_t val = dp_cur[v][r][s];
                if (val == 0) continue;

                int colViol = ((s / 3) == 2) + ((s % 3) == 2);
                if (v + colViol == 2)
                    ans = ans + val;
            }
        }
    }
    return ans;
}
