/*
 * A342510  a(n) = k where Z_k is the largest Zimin word that n (read as a
 *          binary word) does not avoid.
 * https://oeis.org/A342510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the largest k such that the binary representation of n
   contains an instance of the Zimin word Z_k.  The routine uses dynamic
   programming on all substrings of the binary word. */

#include <stdint.h>
#include <string.h>   /* for memcmp */

seq_t a(idx_t n)
{
    /* Convert n to a binary bit array.  bits[0] is the most significant bit. */
    int len;
    char bits[65];

    if (n == 0) {
        len = 1;
        bits[0] = 0;
    } else {
        len = 0;
        uint64_t tmp = (uint64_t)n;
        while (tmp) { len++; tmp >>= 1; }
        for (int i = 0; i < len; i++) {
            bits[i] = (n >> (len - 1 - i)) & 1U;
        }
    }

    /* dp[s][e] = maximum Zimin index for substring bits[s..e] */
    int dp[65][65];
    int best = 1;   /* at least Z_1 for any non‑empty substring */

    for (int i = 0; i < len; i++) {
        for (int j = i; j < len; j++) {
            dp[i][j] = 1;               /* all substrings are at least Z_1 */
        }
    }

    /* Process substrings in increasing length */
    for (int L = 2; L <= len; L++) {
        for (int s = 0; s + L - 1 < len; s++) {
            int e = s + L - 1;
            int val = 1;   /* current best for this substring */

            /* Try all possible split lengths for U (left/right parts) */
            for (int lenU = 1; 2 * lenU < L; lenU++) {
                /* Check if bits[s .. s+lenU-1] == bits[e-lenU+1 .. e] */
                if (memcmp(bits + s, bits + e - lenU + 1, lenU) == 0) {
                    int newk = dp[s][s + lenU - 1] + 1;
                    if (newk > val) val = newk;
                }
            }
            dp[s][e] = val;
            if (val > best) best = val;
        }
    }

    return (seq_t)best;
}
