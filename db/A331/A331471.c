/*
 * A331471  Consider the different ways to split the binary representation of
 *          n into palindromic parts; a(n) is the greatest possible sum of the
 *          parts of such a split.
 * https://oeis.org/A331471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is computed by dynamic programming over the binary *
 *            representation of n. The binary string is split into palindromic
 *            * parts (leading zeros allowed in parts), and the sum of the
 *            values * of the parts is maximized. The function returns this
 *            maximum sum
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* compute bit length L of n (n > 0) */
    idx_t temp = n;
    idx_t L = 0;
    while (temp) {
        temp >>= 1;
        L++;
    }

    /* store bits of n: bits[0] is the most significant bit */
    unsigned char bits[64];
    for (idx_t i = 0; i < L; ++i) {
        bits[i] = (n >> (L - 1 - i)) & 1U;
    }

    /* precompute palindrome table pal[i][j] (inclusive) */
    bool pal[64][64];
    for (idx_t i = 0; i < L; ++i)
        pal[i][i] = true;
    for (idx_t len = 2; len <= L; ++len) {
        for (idx_t i = 0; i + len - 1 < L; ++i) {
            idx_t j = i + len - 1;
            pal[i][j] = (bits[i] == bits[j]) &&
                        (len == 2 || pal[i + 1][j - 1]);
        }
    }

    /* dynamic programming: dp[i] = maximum sum for suffix starting at i */
    seq_t dp[65];
    dp[L] = (seq_t)0;
    for (idx_t i = L; i-- > 0; ) {
        seq_t best = (seq_t)0;
        seq_t val = (seq_t)0;
        for (idx_t j = i; j < L; ++j) {
            val = (val << 1) + bits[j];
            if (pal[i][j]) {
                seq_t cand = val + dp[j + 1];
                if (cand > best)
                    best = cand;
            }
        }
        dp[i] = best;
    }
    return dp[0];
}
