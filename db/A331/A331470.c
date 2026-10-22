/*
 * A331470  a(n) is the greatest value of the form s_1^2 + ... + s_k^2 such
 *          that the concatenation of the binary representations of s_1^2,
 *          ..., s_k^2 equals the binary representation of n.
 * https://oeis.org/A331470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is computed by dynamic programming over the binary
 *            representation of n, splitting it into substrings that are
 *            perfect squares and taking the maximum possible sum of the
 *            squares
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Small integer square root: floor(sqrt(x)) for x <= 64 bits.
   Since n in this sequence is at most 10 000, x is always < 2¹⁴, so
   this naive loop is fast enough. */
static idx_t
isqrt_u64(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Determine the binary length of n and store its bits (MSB→LSB). */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++len;
        tmp >>= 1;
    }

    idx_t bits[64];
    tmp = n;
    int pos = len - 1;
    while (tmp > 0) {
        bits[pos--] = tmp & 1;
        tmp >>= 1;
    }

    /* dp[i] = maximum sum of squares for the suffix starting at bit i. */
    idx_t dp[65];
    dp[len] = 0;

    for (int i = len - 1; i >= 0; --i) {
        idx_t best = 0;
        idx_t val = 0;
        for (int j = i; j < len; ++j) {
            val = (val << 1) | bits[j];
            idx_t r = isqrt_u64(val);
            if (r * r == val) {
                idx_t candidate = val + dp[j + 1];
                if (candidate > best)
                    best = candidate;
            }
        }
        dp[i] = best;
    }

    return (seq_t)dp[0];
}
