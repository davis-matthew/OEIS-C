/*
 * A330956  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the greatest possible value of min(x, y).
 * https://oeis.org/A330956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is computed by exhaustive search of interleavings of * the
 *            binary bits of n, keeping the maximum possible value of min(x,
 *            y)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   dfs_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/dfs_bits.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly */
    if (n == 0) return 0;

    /* Determine bit length L of n */
    idx_t temp = n;
    idx_t L = 0;
    while (temp) {
        ++L;
        temp >>= 1;
    }

    /* Extract bits of n from most significant to least significant */
    uint8_t bits[64];   /* maximum bit length for 64‑bit idx_t */
    for (idx_t i = 0; i < L; ++i) {
        bits[i] = (uint8_t)((n >> (L - 1 - i)) & 1);
    }

    seq_t best = 0;
    dfs_bits(bits, 0, L, 0, 0, &best);
    return best;
}
