/*
 * A293722  Number of distinct nonempty subsequences of the binary expansion
 *          of n.
 * https://oeis.org/A293722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct non‑empty subsequences of the binary
 *            expansion of n Recurrence (dp on prefix): dp[0] = 1 dp[i] =
 *            2*dp[i-1] - dp[last_bit_index-1] (if the bit has appeared
 *            before) Result: a(n) = dp[len] - 1
 * Range:     u8 n <= 1108, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0: binary "0" has exactly one non‑empty subsequence. */
    if (n == 0)
        return (seq_t)1;

    /* Determine the length of the binary representation of n. */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* DP array for counts of distinct subsequences (including the empty one). */
    seq_t dp[65];
    dp[0] = (seq_t)1;

    /* last[0] and last[1] store the last 1‑based position where bit 0/1 was seen. */
    idx_t last[2] = {0, 0};

    for (idx_t pos = 1; pos <= len; ++pos) {
        uint8_t bit = (n >> (len - pos)) & 1u;   /* current bit (msb to lsb) */
        seq_t val = dp[pos - 1] << 1;            /* 2 * dp[pos-1] */

        if (last[bit])                           /* adjust for previous occurrence */
            val -= dp[last[bit] - 1];

        dp[pos] = val;
        last[bit] = pos;                         /* update last occurrence */
    }

    /* Exclude the empty subsequence. */
    return dp[len] - (seq_t)1;
}
