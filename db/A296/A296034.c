/*
 * A296034  Number of n X 3 0..1 arrays with each 1 adjacent to 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS sequence A296034:
   Number of n×3 0..1 arrays with each 1 adjacent to 2 or 4 king‑move
   neighboring 1s.
   The implementation uses a state‑space dynamic programme over rows.
   For every row we keep the previous two rows; once the lower neighbour
   is known we can check that the upper row satisfies the adjacency
   constraints.  This yields an O(n·512) algorithm, easily fast for n up
   to 10 000.
*/

#include <stdbool.h>
#include <stdint.h>

static bool precomp_done = false;
static bool valid[8][8][8];

static void precompute_valid(void)
{
    if (precomp_done) return;
    for (int prev = 0; prev < 8; ++prev) {
        for (int mid  = 0; mid  < 8; ++mid) {
            for (int next = 0; next < 8; ++next) {
                bool ok = true;
                for (int col = 0; col < 3 && ok; ++col) {
                    if (!(mid & (1 << col))) continue;          /* cell is 0 */
                    int cnt = 0;
                    /* up row (prev) */
                    if (col > 0 &&  (prev & (1 << (col-1)))) cnt++;
                    if (prev & (1 << col))                       cnt++;
                    if (col < 2 &&  (prev & (1 << (col+1)))) cnt++;
                    /* same row, other columns */
                    if (col > 0 &&  (mid  & (1 << (col-1)))) cnt++;
                    if (col < 2 &&  (mid  & (1 << (col+1)))) cnt++;
                    /* down row (next) */
                    if (col > 0 &&  (next & (1 << (col-1)))) cnt++;
                    if (next & (1 << col))                       cnt++;
                    if (col < 2 &&  (next & (1 << (col+1)))) cnt++;
                    if (cnt != 2 && cnt != 4) { ok = false; break; }
                }
                valid[prev][mid][next] = ok;
            }
        }
    }
    precomp_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the sequence */
    if (n == 1) return (seq_t)1;    /* only the all‑zero array */

    precompute_valid();

    static seq_t dp[8][8];
    static seq_t nextdp[8][8];
    /* initialise dp with the first two rows; prev row above is 0 */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) dp[i][j] = 0;

    for (int m1 = 0; m1 < 8; ++m1)
        for (int m2 = 0; m2 < 8; ++m2)
            if (valid[0][m1][m2])
                dp[m1][m2] += (seq_t)1;

    /* process rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) nextdp[i][j] = 0;
        for (int prev = 0; prev < 8; ++prev) {
            for (int cur  = 0; cur  < 8; ++cur) {
                seq_t val = dp[prev][cur];
                if (val == 0) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (valid[prev][cur][nxt])
                        nextdp[cur][nxt] += val;
                }
            }
        }
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) dp[i][j] = nextdp[i][j];
    }

    /* finally, validate the last row with a zero below */
    seq_t total = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int cur  = 0; cur  < 8; ++cur) {
            seq_t val = dp[prev][cur];
            if (val == 0) continue;
            if (valid[prev][cur][0]) total += val;
        }
    }
    return total;
}
