/*
 * A251485  Number of (n+1) X (2+1) 0..3 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements less than the minimum of its
 *          antidiagonal elements.
 * https://oeis.org/A251485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the (n+1)×3 arrays with entries 0‑3 such that for
 *            every 2×2 subblock the sum of its main‑diagonal entries is not
 *            less than the minimum of its anti‑diagonal entries. The values
 *            are obtained by dynamic programming over the rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* helper: decode a row integer 0..63 into its three 2‑bit values */
static inline void decode_row(uint8_t r, uint8_t v[3]) {
    v[0] = (r >> 0) & 3;
    v[1] = (r >> 2) & 3;
    v[2] = (r >> 4) & 3;
}

static void build_transition(bool trans[64][64], unsigned char adj[64][64], unsigned char *adj_cnt)
{
    uint8_t prev[3], cur[3];
    for (uint8_t i = 0; i < 64; ++i) {
        decode_row(i, prev);
        for (uint8_t j = 0; j < 64; ++j) {
            decode_row(j, cur);
            bool ok = true;
            for (int col = 0; col < 2; ++col) {
                int a0 = prev[col];
                int a1 = prev[col + 1];
                int b0 = cur[col];
                int b1 = cur[col + 1];
                int diagSum = a0 + b1;                     /* main diagonal sum   */
                int antiMin = a1 < b0 ? a1 : b0;           /* min(anti‑diagonal)  */
                if (diagSum < antiMin) { ok = false; break; }
            }
            trans[i][j] = ok;
        }
    }
    /* build adjacency lists for fast transitions */
    for (uint8_t i = 0; i < 64; ++i) {
        unsigned char cnt = 0;
        for (uint8_t j = 0; j < 64; ++j) {
            if (trans[i][j]) adj[i][cnt++] = j;
        }
        adj_cnt[i] = cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* static once‑init structures */
    static bool ready = false;
    static bool trans[64][64];
    static unsigned char adj[64][64];
    static unsigned char adj_cnt[64];

    if (!ready) {
        build_transition(trans, adj, adj_cnt);
        ready = true;
    }

    seq_t dp[64];
    seq_t ndp[64];

    /* first row counts */
    for (int i = 0; i < 64; ++i) dp[i] = (seq_t)1;

    /* iterate through rows */
    for (idx_t step = 1; step <= n; ++step) {
        for (int j = 0; j < 64; ++j) ndp[j] = (seq_t)0;
        for (int i = 0; i < 64; ++i) {
            seq_t val = dp[i];
            if (val == (seq_t)0) continue;
            unsigned char *list = adj[i];
            unsigned char cnt = adj_cnt[i];
            for (unsigned char k = 0; k < cnt; ++k) {
                ndp[list[k]] += val;
            }
        }
        /* copy ndp back to dp */
        for (int j = 0; j < 64; ++j) dp[j] = ndp[j];
    }

    /* sum all ending rows */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i) total += dp[i];
    return total;
}
