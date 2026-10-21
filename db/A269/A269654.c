/*
 * A269654  Number of length-n 0..6 arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n 0..6 arrays with no repeated adjacent
 *            pair x,x+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, but handle 0 safely */
    if (n == 1) return (seq_t)7;           /* 7 single‑element arrays */

    static seq_t dp[7][64];
    static seq_t nxt[7][64];

    /* initialize dp for length 1 */
    for (idx_t prev = 0; prev < 7; ++prev)
        for (idx_t mask = 0; mask < 64; ++mask)
            dp[prev][mask] = 0;
    for (idx_t val = 0; val < 7; ++val)
        dp[val][0] = 1;

    for (idx_t pos = 2; pos <= n; ++pos) {
        /* zero next array */
        for (idx_t prev = 0; prev < 7; ++prev)
            for (idx_t mask = 0; mask < 64; ++mask)
                nxt[prev][mask] = 0;

        for (idx_t prev = 0; prev < 7; ++prev) {
            for (idx_t mask = 0; mask < 64; ++mask) {
                seq_t cnt = dp[prev][mask];
                if (!cnt) continue;
                for (idx_t nx = 0; nx < 7; ++nx) {
                    if (nx == prev + 1) {                     /* increasing pair x,x+1 */
                        if (mask & (1u << prev)) continue;     /* already repeated */
                        nxt[nx][mask | (1u << prev)] += cnt;
                    } else {
                        nxt[nx][mask] += cnt;
                    }
                }
            }
        }
        /* copy nxt back to dp */
        for (idx_t prev = 0; prev < 7; ++prev)
            for (idx_t mask = 0; mask < 64; ++mask)
                dp[prev][mask] = nxt[prev][mask];
    }

    /* sum all states to get total count */
    seq_t total = 0;
    for (idx_t prev = 0; prev < 7; ++prev)
        for (idx_t mask = 0; mask < 64; ++mask)
            total += dp[prev][mask];
    return total;
}
