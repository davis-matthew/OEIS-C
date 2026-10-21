/*
 * A261473  Number of binary strings of length n+6 such that the smallest
 *          number whose binary representation is not visible in the string is
 *          8.
 * https://oeis.org/A261473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n+6 that contain all *
 *            binary numbers from 0 to 7 as substrings at least once and do
 *            not contain * the binary number 8 (i.e. substring "1000").
 *            Computation is carried out * by a DP over an automaton with 2048
 *            states: (mask of seen patterns, * last up to 3...
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 66, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t N = n + 6; /* length of binary strings to consider */
    /* DP arrays: 2 layers over mask (0..255) and last 3 bits (0..7) */
    seq_t dp[2][256][8] = {{0}};
    int cur = 0, nxt = 1;

    dp[cur][0][0] = (seq_t)1; /* empty string state */

    for (idx_t pos = 0; pos < N; ++pos) {
        /* clear next layer */
        for (int m = 0; m < 256; ++m)
            for (int l = 0; l < 8; ++l)
                dp[nxt][m][l] = (seq_t)0;

        for (int mask = 0; mask < 256; ++mask) {
            for (int last = 0; last < 8; ++last) {
                seq_t cnt = dp[cur][mask][last];
                if (!cnt) continue;

                /* Append bit 0 */
                if (last != 4) { /* avoid forbidden 1000 */
                    int new_last = ((last << 1) | 0) & 7;
                    int new_mask = mask | 1; /* pattern '0' */
                    if (pos + 1 >= 2) {
                        int two = (last & 1) << 1; /* two_bits with d=0 */
                        if (two == 2) new_mask |= 4;   /* '10'  */
                        else if (two == 3) new_mask |= 8; /* '11'  */
                    }
                    if (pos + 1 >= 3) {
                        if (new_last == 4) new_mask |= 16; /* '100' */
                        else if (new_last == 5) new_mask |= 32; /* '101' */
                        else if (new_last == 6) new_mask |= 64; /* '110' */
                        else if (new_last == 7) new_mask |= 128; /* '111' */
                    }
                    dp[nxt][new_mask][new_last] += cnt;
                }

                /* Append bit 1 */
                {
                    int new_last = ((last << 1) | 1) & 7;
                    int new_mask = mask | 2; /* pattern '1' */
                    if (pos + 1 >= 2) {
                        int two = ((last & 1) << 1) | 1; /* two_bits with d=1 */
                        if (two == 2) new_mask |= 4;   /* '10'  */
                        else if (two == 3) new_mask |= 8; /* '11'  */
                    }
                    if (pos + 1 >= 3) {
                        if (new_last == 4) new_mask |= 16; /* '100' */
                        else if (new_last == 5) new_mask |= 32; /* '101' */
                        else if (new_last == 6) new_mask |= 64; /* '110' */
                        else if (new_last == 7) new_mask |= 128; /* '111' */
                    }
                    dp[nxt][new_mask][new_last] += cnt;
                }
            }
        }
        /* swap layers */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum over all endings with all patterns seen (mask == 0xFF) */
    seq_t ans = (seq_t)0;
    for (int last = 0; last < 8; ++last)
        ans += dp[cur][255][last];

    return ans;
}
