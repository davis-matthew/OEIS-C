/*
 * A253307  Number of (n+1)X(1+1) 0..2 arrays with every 2X2 subblock diagonal
 *          maximum minus antidiagonal maximum unequal to its neighbors
 *          horizontally, vertically, diagonally and antidiagonally.
 * https://oeis.org/A253307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows.  
   For each n >= 1, the function counts the number of (n+1)×2 arrays with
   entries in {0,1,2} such that for every consecutive pair of rows
   r_k, r_{k+1} the value  
      diff(r_k,r_{k+1}) = max(r_k[0],r_{k+1}[1]) - max(r_k[1],r_{k+1}[0])
   differs from the diff of the previous pair (r_{k-1},r_k).  This
   directly implements the requirement that each 2×2 block’s diff be
   distinct from its vertical neighbours (the only neighbours in a 2‑column
   array).  The algorithm is O(n) with a tiny constant factor and works
   for all values of seq_t and idx_t. */

/* number of possible rows (3²) */
enum { ROWS = 9, DIFFS = 5 };          /* DIFFS=5 for diffs –2..2 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)                /* behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    /* mapping for row encoding: r = 3*first_digit + second_digit */
    int first[ROWS];
    int second[ROWS];
    for (int r = 0; r < ROWS; ++r) {
        first[r]  = r / 3;
        second[r] = r % 3;
    }

    /* precompute diff between any two rows */
    int diff[ROWS][ROWS];
    for (int r0 = 0; r0 < ROWS; ++r0)
        for (int r1 = 0; r1 < ROWS; ++r1) {
            int main_max  = first[r0] > second[r1] ? first[r0] : second[r1];
            int anti_max  = second[r0] > first[r1] ? second[r0] : first[r1];
            diff[r0][r1] = main_max - anti_max;          /* –2 .. 2 */
        }

    /* DP: dp[r][d] = number of arrays ending with row r
                    and last diff value d-2 (0..4)    */
    static seq_t dp[ROWS][DIFFS];
    static seq_t ndp[ROWS][DIFFS];

    /* initialise dp for the first block (rows r0,r1) */
    for (int r = 0; r < ROWS; ++r)
        for (int d = 0; d < DIFFS; ++d)
            dp[r][d] = 0;

    for (int r0 = 0; r0 < ROWS; ++r0)
        for (int r1 = 0; r1 < ROWS; ++r1) {
            int d = diff[r0][r1] + 2;    /* shift to 0..4 */
            dp[r1][d] += (seq_t)1;
        }

    /* process remaining rows; step counts the current number of rows processed */
    for (idx_t step = 2; step <= n; ++step) {
        for (int r = 0; r < ROWS; ++r)
            for (int d = 0; d < DIFFS; ++d)
                ndp[r][d] = 0;

        for (int prev = 0; prev < ROWS; ++prev)
            for (int pd = 0; pd < DIFFS; ++pd) {
                seq_t cnt = dp[prev][pd];
                if (cnt == 0) continue;
                for (int nxt = 0; nxt < ROWS; ++nxt) {
                    int nd = diff[prev][nxt] + 2;
                    if (nd == pd) continue;   /* diff must differ from previous */
                    ndp[nxt][nd] += cnt;
                }
            }

        /* copy ndp back to dp */
        for (int r = 0; r < ROWS; ++r)
            for (int d = 0; d < DIFFS; ++d)
                dp[r][d] = ndp[r][d];
    }

    /* total count */
    seq_t total = 0;
    for (int r = 0; r < ROWS; ++r)
        for (int d = 0; d < DIFFS; ++d)
            total += dp[r][d];

    return total;
}
