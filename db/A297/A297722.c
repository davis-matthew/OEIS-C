/*
 * A297722  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 4 neighboring 1s.
 * https://oeis.org/A297722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes the sequence A297722: the number of 3×n 0/1 arrays in
    which every '1' has 1, 2 or 4 neighbors among the horizontal and
    diagonal positions. The algorithm builds a transition table for all
    8 possible 3‑bit columns and uses a dynamic programming sweep over
    the columns of the array.  The state consists of the last two
    columns; when a new column is appended we verify that the middle
    column satisfies the neighbour condition.  Finally the last column
    is validated against a trailing column of all zeros.  The DP runs
    in time O(n·8³).  */
#include <stdbool.h>
#include <stdint.h>

static bool valid[8][8][8];
static bool transition_computed = false;

/* Pre‑compute the 8³ transitions.  For a given triple of columns
   (left, middle, right) the function checks whether every '1' in
   the middle column has 1, 2 or 4 neighbors among the horizontal
   and diagonal positions.  Vertical neighbours are not counted. */
static void compute_transition(void)
{
    if (transition_computed) return;
    for (int left = 0; left < 8; ++left)
        for (int mid = 0; mid < 8; ++mid)
            for (int right = 0; right < 8; ++right) {
                bool ok = true;
                for (int r = 0; r < 3 && ok; ++r)
                    if ((mid >> r) & 1) { /* this cell is 1 */
                        int cnt = 0;
                        /* neighbours from the left column */
                        if ((left >> r) & 1)                 cnt++;
                        if (r > 0   && ((left >> (r-1)) & 1)) cnt++;
                        if (r < 2   && ((left >> (r+1)) & 1)) cnt++;
                        /* neighbours from the right column */
                        if ((right >> r) & 1)                 cnt++;
                        if (r > 0   && ((right >> (r-1)) & 1)) cnt++;
                        if (r < 2   && ((right >> (r+1)) & 1)) cnt++;
                        if (cnt != 1 && cnt != 2 && cnt != 4) ok = false;
                    }
                valid[left][mid][right] = ok;
            }
    transition_computed = true;
}

seq_t a(idx_t n)
{
    /* No array of width 0 participates in the definition. */
    if (n == 0) return (seq_t)0;

    compute_transition();

    /* dp[prev][curr] = number of ways to build the already processed
       part of the array with the last two columns equal to
       (prev, curr).  Indices 0…7 correspond to 3‑bit columns. */
    seq_t dp[8][8] = {0};
    seq_t newdp[8][8];

    /* First column – start with all possible columns, no
       validation yet (the left neighbour will be 0). */
    for (int curr = 0; curr < 8; ++curr)
        dp[0][curr] = (seq_t)1;

    /* Add columns 2..n.  After adding a column 'next', we validate
       the previous column 'curr' against its left (prev) and right
       (next) neighbours. */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                newdp[i][j] = (seq_t)0;

        for (int prev = 0; prev < 8; ++prev)
            for (int curr = 0; curr < 8; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (!cnt) continue;
                for (int next = 0; next < 8; ++next)
                    if (valid[prev][curr][next])
                        newdp[curr][next] = newdp[curr][next] + cnt;
            }
        /* move to the next step */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Final validation: the last column must satisfy the condition
       with a trailing column of all zeros. */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 8; ++prev)
        for (int curr = 0; curr < 8; ++curr)
            if (dp[prev][curr] && valid[prev][curr][0])
                result = result + dp[prev][curr];

    return result;
}
