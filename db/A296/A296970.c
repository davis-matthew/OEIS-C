/*
 * A296970  Number of nX4 0..1 arrays with each 1 adjacent to 2, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296970: count of n×4 binary arrays where every 1 has
   exactly 2, 4, or 5 king‑move neighbours that are 1.
   The implementation uses a state DP over consecutive rows:
   dp[up][mid] = number of arrays of rows processed so far, where
   the last two rows are `up` (row i‑2) and `mid` (row i‑1).
   For each new row `down` we check that every 1 in the middle row
   has a valid neighbour count using the triple (up,mid,down).
   This gives O(n) time and O(1) memory (the DP tables are 16×16).   */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* validity table: valid[up][mid][down] is true iff
   every 1 in 'mid' has 2, 4 or 5 1‑neighbours in the 3×4 block
   formed by rows up, mid and down. */
static unsigned char valid[16][16][16];
static bool valid_init = false;

/* precompute the table once */
static void init_valid_table(void)
{
    for (int up = 0; up < 16; ++up)
        for (int mid = 0; mid < 16; ++mid)
            for (int down = 0; down < 16; ++down) {
                bool ok = true;
                if (mid == 0) {       /* no 1's, always OK */
                    ok = true;
                } else {
                    for (int c = 0; c < 4 && ok; ++c) {
                        if (!(mid >> c & 1)) continue;   /* no 1 at this column */
                        int cnt = 0;
                        /* same row */
                        if (c > 0)   cnt += (mid >> (c-1)) & 1;
                        if (c < 3)   cnt += (mid >> (c+1)) & 1;
                        /* upper row */
                        if (c > 0)   cnt += (up  >> (c-1)) & 1;
                        cnt += (up  >> c) & 1;
                        if (c < 3)   cnt += (up  >> (c+1)) & 1;
                        /* lower row */
                        if (c > 0)   cnt += (down>> (c-1)) & 1;
                        cnt += (down>> c) & 1;
                        if (c < 3)   cnt += (down>> (c+1)) & 1;
                        if (!(cnt == 2 || cnt == 4 || cnt == 5))
                            ok = false;
                    }
                }
                valid[up][mid][down] = ok;
            }
    valid_init = true;
}

seq_t a(idx_t n)
{
    if (!valid_init) init_valid_table();

    /* DP tables: old and new rows */
    seq_t dp[16][16];
    seq_t newdp[16][16];
    /* initialise dp with the first row above being all zeros */
    for (int up = 0; up < 16; ++up)
        for (int mid = 0; mid < 16; ++mid)
            dp[up][mid] = (up == 0) ? (mid ? 1 : 1) : 0;  /* actually dp[0][mid]=1 */
    for (int up = 1; up < 16; ++up)
        for (int mid = 0; mid < 16; ++mid)
            dp[up][mid] = 0;

    /* process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero newdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = 0;

        for (int up = 0; up < 16; ++up) {
            for (int mid = 0; mid < 16; ++mid) {
                seq_t ways = dp[up][mid];
                if (!ways) continue;
                for (int down = 0; down < 16; ++down) {
                    if (!valid[up][mid][down]) continue;
                    newdp[mid][down] += ways;
                }
            }
        }
        /* copy newdp → dp (element‑wise assignment, no memcpy) */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* final check: last row has no row below (treated as 0) */
    seq_t result = 0;
    for (int up = 0; up < 16; ++up)
        for (int mid = 0; mid < 16; ++mid)
            if (valid[up][mid][0])
                result += dp[up][mid];

    return result;
}
