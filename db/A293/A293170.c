/*
 * A293170  Number of distinct subsequences of the binary expansion of n.
 * https://oeis.org/A293170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = number of distinct subsequences of the binary
 *            representation of n
 * Range:     u8 n <= 1108, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Determine the bit length of n; treat 0 as a single bit '0'. */
    idx_t bitlen = (n == 0) ? 1 : floor_log2_uint64((uint64_t)n) + 1;

    /* dp[i] = number of distinct subsequences of the first i bits (i from 0 to bitlen). */
    seq_t dp[65];   /* 64 bits + 1 for the empty prefix; array size safe for all idx_t values. */
    dp[0] = 1;      /* only the empty subsequence */

    /* lastPos[0] = position (1‑based) of the last seen '0', 0 if none; same for '1'. */
    int lastPos[2] = {0, 0};

    for (idx_t i = 0; i < bitlen; ++i) {
        /* Extract the i‑th bit from most significant to least significant. */
        int b = (int)((n >> (bitlen - 1 - i)) & 1);

        int pos = (int)i + 1;                /* current prefix length */
        seq_t subtract = (lastPos[b] == 0) ? 0 : dp[lastPos[b] - 1];

        dp[pos] = (dp[pos - 1] * ((seq_t)2)) - subtract;

        lastPos[b] = pos;                    /* update last position of this bit */
    }

    return dp[bitlen];
}
