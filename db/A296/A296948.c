/*
 * A296948  Number of n X 4 0..1 arrays with each 1 adjacent to 1, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×4 0..1 arrays with each 1 adjacent to 1,3 or 5
 *            king-move neighboring 1s. Computed by a transfer-matrix DP over
 *            rows
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether the middle row 'mid' with neighbors 'top' and 'bottom'
   satisfies the adjacency condition for all 1s in 'mid'. */
static inline bool valid_row(uint8_t top, uint8_t mid, uint8_t bottom) {
    /* For every column c (0..3) */
    for (int c = 0; c < 4; ++c) {
        if (!((mid >> c) & 1)) continue;    /* cell is 0, no restriction */
        int count = 0;
        /* Scan all king-move neighbors */
        for (int dr = -1; dr <= 1; ++dr) {
            uint8_t row_bits = (dr == -1) ? top
                            : (dr == 0)  ? mid
                            : bottom;     /* dr == 1 */
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;   /* skip self */
                int cc = c + dc;
                if (cc < 0 || cc >= 4) continue;    /* outside grid */
                if ((row_bits >> cc) & 1) ++count;
            }
        }
        /* The required counts are 1, 3 or 5 */
        if (!(count == 1 || count == 3 || count == 5)) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* 4-bit patterns: 0..15 */
    const int N = 16;
    /* dp and next arrays of size 16*16 (index = prev*16 + curr) */
    static seq_t dp[256], nextdp[256];
    /* initialize all to 0 */
    for (int i = 0; i < 256; ++i) dp[i] = 0;

    /* base case: first row, previous row is all zeros (0) */
    for (int curr = 0; curr < N; ++curr) dp[0 * N + curr] = 1;

    /* Process rows 2..n */
    for (idx_t row = 1; row < n; ++row) {
        /* zero nextdp */
        for (int i = 0; i < 256; ++i) nextdp[i] = 0;
        for (int prev = 0; prev < N; ++prev) {
            for (int curr = 0; curr < N; ++curr) {
                seq_t ways = dp[(prev * N) + curr];
                if (ways == 0) continue;
                for (int next = 0; next < N; ++next) {
                    if (valid_row((uint8_t)prev, (uint8_t)curr, (uint8_t)next)) {
                        nextdp[(curr * N) + next] += ways;
                    }
                }
            }
        }
        /* copy nextdp back to dp */
        for (int i = 0; i < 256; ++i) dp[i] = nextdp[i];
    }

    /* final validation for the last row (no row below) */
    seq_t total = 0;
    for (int prev = 0; prev < N; ++prev) {
        for (int curr = 0; curr < N; ++curr) {
            seq_t ways = dp[(prev * N) + curr];
            if (ways == 0) continue;
            if (valid_row((uint8_t)prev, (uint8_t)curr, 0)) {
                total += ways;
            }
        }
    }
    return total;
}
