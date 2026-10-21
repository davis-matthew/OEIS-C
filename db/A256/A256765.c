/*
 * A256765  Number of (n+2)X(2+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum less than the antidiagonal sum.
 * https://oeis.org/A256765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 binary arrays with no 3×3 subblock
 *            whose main diagonal sum is less than the antidiagonal sum. The
 *            sequence is computed by dynamic programming over rows. Every
 *            pair of consecutive rows (prev2,prev1) is encoded as a 8‑bit
 *            state; there are 256 states. For each state we preco...
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: return bit pos of a 4‑bit pattern (pos 0 is leftmost column). */
static inline int bit(int pattern, int pos)
{
    return (pattern >> pos) & 1;
}

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n < 1 */
        return 0;

    /* Transition tables: next_states[p][t] stores the (prev1,row) state
       reachable from state p by adding a new row; next_cnt[p] is the
       number of such transitions.  These tables are built once. */
    static bool init = false;
    static uint8_t next_states[256][16];
    static uint8_t next_cnt[256];

    /* DP tables for the current and next rows.  All 256 states are used. */
    static seq_t dp[256];
    static seq_t dp_next[256];

    /* Build transition tables lazily. */
    if (!init) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                int state = (i << 4) | j;   /* encode (i,j) */
                uint8_t c = 0;
                for (int k = 0; k < 16; ++k) {
                    /* Main/antidiagonal sums for the two possible 3×3 blocks. */
                    int m0 = bit(i,0) + bit(j,1) + bit(k,2);
                    int a0 = bit(i,2) + bit(j,1) + bit(k,0);
                    if (m0 < a0) continue;
                    int m1 = bit(i,1) + bit(j,2) + bit(k,3);
                    int a1 = bit(i,3) + bit(j,2) + bit(k,1);
                    if (m1 < a1) continue;
                    /* Allowed transition: (i,j) -> (j,k) */
                    next_states[state][c++] = (j << 4) | k;
                }
                next_cnt[state] = c;
            }
        }
        init = true;
    }

    /* Initialization: after the first two rows all 256 state pairs are
       possible, each with count 1. */
    for (int idx = 0; idx < 256; ++idx)
        dp[idx] = 0;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[(i << 4) | j] = 1;

    /* Total number of rows is n + 2.  Process the remaining
       n rows (from the third up to the last). */
    idx_t rows = n + 2;
    for (idx_t r = 3; r <= rows; ++r) {
        for (int idx = 0; idx < 256; ++idx)
            dp_next[idx] = 0;
        for (int idx = 0; idx < 256; ++idx) {
            seq_t cur = dp[idx];
            if (cur == 0) continue;
            uint8_t c = next_cnt[idx];
            for (uint8_t t = 0; t < c; ++t) {
                int next_idx = next_states[idx][t];
                dp_next[next_idx] += cur;
            }
        }
        for (int idx = 0; idx < 256; ++idx)
            dp[idx] = dp_next[idx];
    }

    /* Sum over all ending states to get the total number of arrays. */
    seq_t sum = 0;
    for (int idx = 0; idx < 256; ++idx)
        sum += dp[idx];
    return sum;
}
