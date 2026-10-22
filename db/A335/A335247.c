/*
 * A335247  a(n) is the number of binary (0,1) sequences of length n that have
 *          at least two ones in each window of eight consecutive symbols.
 * https://oeis.org/A335247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335247: number of binary strings of length n having at least two
 * 1's in every window of eight consecutive symbols.
 *
 * Implementation: dynamic programming over the last 7 bits (128 states).
 * For n < 7 all strings are allowed (2^n).  For n = 7 we exclude the all‑zero
 * string (127 strings).  For n ≥ 8 we keep a DP that enforces the window
 * constraint.  The implementation works with any unsigned integer type
 * provided for seq_t, and with arbitrary‑precision types in a C++17 build.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle small n by direct formula */
    if (n < 7) {
        seq_t v = (seq_t)1;
        for (idx_t i = 0; i < n; ++i) v <<= 1;
        return v;
    }
    if (n == 7) return (seq_t)127;

    /* DP over last 7 bits: dp[mask] = count of strings ending with mask */
    seq_t dp[128];
    seq_t next[128];
    for (size_t i = 0; i < 128; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Precompute popcount for 0..127 once */
    static unsigned char popcnt[128];
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < 128; ++i) {
            unsigned int x = (unsigned int)i, c = 0;
            while (x) { c += x & 1; x >>= 1; }
            popcnt[i] = (unsigned char)c;
        }
        initialized = 1;
    }

    unsigned int cur_len = 0;
    for (idx_t step = 0; step < n; ++step) {
        for (size_t i = 0; i < 128; ++i) next[i] = 0;
        for (unsigned int mask = 0; mask < 128; ++mask) {
            seq_t val = dp[mask];
            if (val == 0) continue;
            for (unsigned int b = 0; b < 2; ++b) {
                /* If the string length before adding this bit is at least 7,
                 * we must check the last 8 bits (mask + new bit). */
                if (cur_len >= 7 && (unsigned int)(popcnt[mask] + b) < 2)
                    continue;
                unsigned int new_mask = ((mask << 1) | b) & 0x7F;
                next[new_mask] += val;
            }
        }
        for (size_t i = 0; i < 128; ++i) dp[i] = next[i];
        ++cur_len;
    }

    seq_t total = 0;
    for (size_t i = 0; i < 128; ++i) total += dp[i];
    return total;
}
