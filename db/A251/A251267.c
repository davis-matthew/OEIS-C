/*
 * A251267  Number of (n+1)X(7+1) 0..1 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×8 binary arrays with no 2×2 subblock
 *            having x11-x00 < x10-x01. Computed via dynamic programming over
 *            8‑bit row states; adjacency graph built once, then counted as
 *            walks of length n
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Number of possible rows (8 bits) */
#define ROWS 256

static unsigned char adj_len[ROWS];
static unsigned char adj_rows[ROWS][ROWS];

/* Build adjacency: for each row r, store all rows c that can follow r. */
static void build_adjacency(void)
{
    for (int r = 0; r < ROWS; ++r) {
        unsigned char *nbr = adj_rows[r];
        unsigned char cnt = 0;
        for (int c = 0; c < ROWS; ++c) {
            bool ok = true;
            for (int j = 0; j < 7; ++j) {
                int a0 = (r >> j) & 1;
                int a1 = (r >> (j + 1)) & 1;
                int b0 = (c >> j) & 1;
                int b1 = (c >> (j + 1)) & 1;
                int left  = b1 - a0;   /* x11 - x00 */
                int right = b0 - a1;   /* x10 - x01 */
                if (left < right) {    /* forbidden */
                    ok = false;
                    break;
                }
            }
            if (ok) nbr[cnt++] = (unsigned char)c;
        }
        adj_len[r] = cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Sequence is defined for n ≥ 1 */

    static bool built = false;
    if (!built) {
        build_adjacency();
        built = true;
    }

    seq_t dp[ROWS];
    seq_t newdp[ROWS];
    const seq_t one = (seq_t)1;
    for (int i = 0; i < ROWS; ++i) dp[i] = one;

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < ROWS; ++i) newdp[i] = (seq_t)0;
        for (int r = 0; r < ROWS; ++r) {
            seq_t val = dp[r];
            unsigned char *nbr = adj_rows[r];
            unsigned char cnt = adj_len[r];
            for (unsigned char k = 0; k < cnt; ++k) {
                unsigned char c = nbr[k];
                newdp[c] += val;
            }
        }
        for (int i = 0; i < ROWS; ++i) dp[i] = newdp[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < ROWS; ++i) total += dp[i];
    return total;
}
