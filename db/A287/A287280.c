/*
 * A287280  Number of set partitions of [n] such that for each block all
 *          absolute differences between consecutive elements are <= eight.
 * https://oeis.org/A287280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A287214(n,8) Computed by dynamic programming on block
 *            "last element" distances. For each prefix 1..i, the state
 *            bitmask encodes blocks whose last element lies in {i-1, i-2, …,
 *            i-8}. Bit d-1 corresponds to a block whose last element is
 *            i-(d). Transition for element i+1 considers assigning t...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Limit for 8‑bit masks */
    const unsigned int LIM = 0xFFu;
    seq_t dp[256];
    seq_t nxt[256];

    /* initialize dp */
    for (int i = 0; i < 256; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 256; ++i) nxt[i] = 0;

        for (int mask = 0; mask < 256; ++mask) {
            seq_t ways = dp[mask];
            if (ways == 0) continue;

            /* Shift bits: old last at i-1 becomes distance 1 to new i */
            unsigned int s = (unsigned int)mask << 1;   /* bits 1…8 contain eligible blocks */

            /* 1. start a new block containing only the new element */
            unsigned int nm = (s & LIM) | 1u;          /* keep only bits 0…7, set bit 0 */
            nxt[nm] += ways;

            /* 2. merge the new element into each eligible block */
            for (int d = 1; d <= 8; ++d) {
                if (s & (1u << d)) {
                    unsigned int nm2 = (s & ~(1u << d)) & LIM;  /* clear used block, keep bits 0…7 */
                    nm2 |= 1u;                                 /* new block at current element */
                    nxt[nm2] += ways;
                }
            }
        }

        /* move to next step */
        for (int i = 0; i < 256; ++i) dp[i] = nxt[i];
    }

    /* sum of all end states gives the total number of partitions */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += dp[i];

    return total;
}
