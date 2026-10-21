/*
 * A287281  Number of set partitions of [n] such that for each block all
 *          absolute differences between consecutive elements are <= nine.
 * https://oeis.org/A287281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A287281 – Number of set partitions of [n] with all consecutive
 * element differences ≤ 9.  Computed by dynamic programming on a
 * 10‑bit mask that records, for each block, the distance of its last
 * element from the current position. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 10‑bit mask: bits 0..9 correspond to distances 0..9 from current n. */
    #define MASKS (1U << 10)

    seq_t dp[MASKS];
    seq_t ndp[MASKS];

    /* initialize for n = 0 */
    for (size_t i = 0; i < MASKS; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t pos = 0; pos < n; ++pos) {
        /* clear ndp for the next step */
        for (size_t i = 0; i < MASKS; ++i) ndp[i] = 0;

        for (size_t mask = 0; mask < MASKS; ++mask) {
            seq_t cnt = dp[mask];
            if (cnt == 0) continue;

            /* shift existing blocks one step farther from the new element */
            size_t base = (mask << 1) & (MASKS - 1);

            /* 1. start a new block containing the new element */
            ndp[base | 1] += cnt;

            /* 2. extend an existing block whose last element is at distance
             *    0..8 from the previous position */
            for (int d = 0; d <= 8; ++d) {
                if (mask & ((size_t)1 << d)) {
                    /* block j: clear its old bit (d+1) and set bit 0 */
                    size_t next = (base | 1) & ~((size_t)1 << (d + 1));
                    ndp[next] += cnt;
                }
            }
        }

        /* copy ndp into dp for the next iteration */
        for (size_t i = 0; i < MASKS; ++i) dp[i] = ndp[i];
    }
    #undef MASKS

    /* sum all states to obtain the total number of partitions */
    seq_t ans = 0;
    for (size_t i = 0; i < 1024; ++i) ans += dp[i];
    return ans;
}
