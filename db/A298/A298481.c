/*
 * A298481  a(n) is the number of ways to partition the binary representation
 *          of n into the minimal number of palindromic parts.
 * https://oeis.org/A298481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of ways to partition the binary representation of
 *            n into the minimal number of palindromic parts
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but guard */

    /* Determine the length of the binary representation */
    idx_t len = floor_log2_uint64(n) + 1;   /* number of bits, 1 <= len <= 64 */

    /* bits[0] is the most significant bit */
    unsigned char bits[65];   /* 64 bits + 1 for safety */
    int i;
    for (i = 0; i < (int)len; ++i) {
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1u);
    }

    /* DP arrays: dp[i] = minimal number of palindromic parts for prefix of length i
       ways[i] = number of ways to achieve that minimal number for that prefix */
    int dp[65];
    seq_t ways[65];
    dp[0] = 0;
    ways[0] = (seq_t)1;

    for (i = 1; i <= (int)len; ++i) {
        dp[i] = (int)len;        /* large initial value */
        ways[i] = (seq_t)0;
        for (int j = 0; j < i; ++j) {
            /* check palindrome of bits[j .. i-1] */
            bool pal = true;
            int lo = j, hi = i - 1;
            while (lo < hi) {
                if (bits[lo] != bits[hi]) {
                    pal = false;
                    break;
                }
                ++lo;
                --hi;
            }
            if (!pal) continue;

            int cand = dp[j] + 1;
            if (cand < dp[i]) {
                dp[i] = cand;
                ways[i] = ways[j];
            } else if (cand == dp[i]) {
                ways[i] = ways[i] + ways[j];
            }
        }
    }

    return ways[(int)len];
}
