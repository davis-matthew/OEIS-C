/*
 * A250076  Number of length n+5 0..3 arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 15, u64 n <= 37, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A250076: Number of length n+5 0..3 arrays with every six consecutive terms
 * having the maximum of some two terms equal to the minimum of the remaining four.
 * Implementation uses a dynamic programming over 5‑digit states (length 5 prefixes).
 * There are 4⁵ = 1024 states; transitions are precomputed by checking each 6‑tuple
 * for the property.
 * Complexity: O(n * 4⁵) with very small constant.
 */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute the allowed transitions once */
    static bool allowed[1024][4] = {0};
    static bool initialized = false;
    if (!initialized) {
        for (idx_t s = 0; s < 1024; ++s) {
            uint8_t d[5];
            idx_t tmp = s;
            for (int i = 4; i >= 0; --i) {
                d[i] = tmp & 3;          /* base‑4 digit */
                tmp >>= 2;
            }
            for (int x = 0; x < 4; ++x) {
                uint8_t w[6];
                for (int k = 0; k < 5; ++k) w[k] = d[k];
                w[5] = (uint8_t)x;
                bool ok = false;
                for (int i = 0; i < 6 && !ok; ++i) {
                    for (int j = i + 1; j < 6 && !ok; ++j) {
                        uint8_t mx = (w[i] > w[j]) ? w[i] : w[j];
                        uint8_t mn = 4;            /* larger than max possible value */
                        for (int k = 0; k < 6; ++k) {
                            if (k == i || k == j) continue;
                            if (w[k] < mn) mn = w[k];
                        }
                        if (mx == mn) ok = true;
                    }
                }
                allowed[s][x] = ok;
            }
        }
        initialized = true;
    }

    /* DP over length positions */
    const idx_t STATES = 1024;
    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];

    /* Initial state: any 5‑tuple is allowed (empty prefix) */
    for (idx_t s = 0; s < STATES; ++s) dp_cur[s] = (seq_t)1;

    idx_t N = n + (idx_t)5;              /* total length required */
    for (idx_t pos = 6; pos <= N; ++pos) {
        for (idx_t s = 0; s < STATES; ++s) dp_next[s] = (seq_t)0;
        for (idx_t s = 0; s < STATES; ++s) {
            seq_t ways = dp_cur[s];
            if (ways == (seq_t)0) continue;
            for (int x = 0; x < 4; ++x) {
                if (!allowed[s][x]) continue;
                idx_t ns = ((s & 255) << 2) | (idx_t)x;  /* shift state left, add x */
                dp_next[ns] += ways;
            }
        }
        for (idx_t s = 0; s < STATES; ++s) dp_cur[s] = dp_next[s];
    }

    /* Sum all end states */
    seq_t total = (seq_t)0;
    for (idx_t s = 0; s < STATES; ++s) total += dp_cur[s];
    return total;
}
