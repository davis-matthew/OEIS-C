/*
 * A330090  Inverse permutation to A330081.
 * https://oeis.org/A330090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bitwise shuffle of the binary digits of n: If n has
 *            binary expansion (b1, ... , bw) with b1 the most‑significant
 *            bit, then a(n) has binary expansion (b1, bw, b2, bw-1, b3, bw-2,
 *            ...)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* a(0) = 0 */
        return 0;

    /* Compute the length of the binary representation of n. */
    const int w = 64 - __builtin_clzll((unsigned long long)n);  /* w >= 1 */

    seq_t result = 0;
    for (int i = 0; i < w; ++i)
    {
        /* Destination bit position i (MSB first). */
        int src_index = (i % 2 == 0) ? (i / 2) : (w - 1 - (i / 2));
        /* Extract the corresponding bit from n. */
        int bit = (int)((n >> (w - 1 - src_index)) & 1ull);
        result = (result << 1) | bit;
    }

    return result;
}
