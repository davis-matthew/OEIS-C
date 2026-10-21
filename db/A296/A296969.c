/*
 * A296969  Number of nX3 0..1 arrays with each 1 adjacent to 2, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 0..1 arrays with each 1 adjacent to 2, 4 or
 *            5 king‑move neighboring 1s. * Computed using dynamic programming
 *            over rows: for each state (previous row, current row) we * keep
 *            the number of arrays consistent up to the current row. The
 *            transition checks that * the current row sat...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper tables, built once */
static bool valid[8][8][8];
static bool valid_no[8];
static bool tables_done = false;

static void build_tables(void)
{
    if(tables_done) return;
    tables_done = true;

    /* pre‑compute validity of a row given its above and below neighbours */
    for(int prev = 0; prev < 8; ++prev)
    for(int curr = 0; curr < 8; ++curr)
    for(int next = 0; next < 8; ++next)
    {
        bool ok = true;
        /* examine each 1 in the current mask */
        for(int j = 0; j < 3; ++j)
        {
            if(!(curr & (1 << j))) continue;          /* not a 1 */

            int cnt = 0;
            /* neighbors above */
            if(j>0 && (prev & (1 << (j-1)))) ++cnt;
            if(      prev & (1 << j))             ++cnt;
            if(j<2 && (prev & (1 << (j+1)))) ++cnt;
            /* neighbors in the same row */
            if(j>0 && (curr & (1 << (j-1)))) ++cnt;
            if(j<2 && (curr & (1 << (j+1)))) ++cnt;
            /* neighbors below */
            if(j>0 && (next & (1 << (j-1)))) ++cnt;
            if(      next & (1 << j))             ++cnt;
            if(j<2 && (next & (1 << (j+1)))) ++cnt;

            if(cnt != 2 && cnt != 4 && cnt != 5)
            {
                ok = false;
                break;
            }
        }
        valid[prev][curr][next] = ok;
    }
    /* validity when isolated (no neighbours on either side) */
    for(int m = 0; m < 8; ++m) valid_no[m] = valid[0][m][0];
}

/* ------------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    build_tables();

    /* n is at least 1 by problem statement */
    if(n == 1)
    {
        seq_t res = 0;
        for(int m = 0; m < 8; ++m)
            if(valid_no[m]) ++res;
        return res;
    }

    if(n == 2)
    {
        seq_t res = 0;
        for(int r1 = 0; r1 < 8; ++r1)
        for(int r2 = 0; r2 < 8; ++r2)
            if(valid[0][r1][r2] && valid[r1][r2][0]) ++res;
        return res;
    }

    /* n >= 3 */
    seq_t dp[8][8] = {{0}};
    seq_t ndp[8][8];

    /* initialise states for first two rows; first row must already be valid
       with its below neighbour (the second row). */
    for(int r1 = 0; r1 < 8; ++r1)
    for(int r2 = 0; r2 < 8; ++r2)
        if(valid[0][r1][r2]) dp[r1][r2] = 1;

    for(idx_t row = 3; row <= n; ++row)
    {
        /* zero ndp */
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                ndp[i][j] = 0;

        for(int prev = 0; prev < 8; ++prev)
        for(int curr = 0; curr < 8; ++curr)
        {
            seq_t ways = dp[prev][curr];
            if(!ways) continue;
            for(int next = 0; next < 8; ++next)
                if(valid[prev][curr][next])
                    ndp[curr][next] += ways;
        }
        /* copy ndp to dp */
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* after processing all rows, last row must be valid with no successor */
    seq_t result = 0;
    for(int prev = 0; prev < 8; ++prev)
    for(int curr = 0; curr < 8; ++curr)
        if(valid[prev][curr][0]) result += dp[prev][curr];

    return result;
}
