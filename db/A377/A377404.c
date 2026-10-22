/*
 * A377404  In the binary expansion of n, replace the first, third, fifth,
 *          etc. 1's by 0's.
 * https://oeis.org/A377404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n - A265263(n) – keep the 2nd, 4th, … 1's in the binary
 *            expansion of n (scanning from the most significant bit)
 * Range:     u8 n <= 767, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Bit width of the result type */
    const unsigned width = (unsigned)(sizeof(seq_t) << 3);
    seq_t res = 0;
    unsigned ones_seen = 0;

    /* Position of the most significant set bit of n */
    int msb = 63 - __builtin_clzll((uint64_t)n);

    for (int i = msb; i >= 0; --i) {
        if ((n >> i) & 1ULL) {
            ++ones_seen;
            /* keep even‑indexed 1's (2nd, 4th, …) */
            if ((ones_seen & 1) == 0 && (unsigned)i < width) {
                res |= ((seq_t)1) << i;
            }
        }
    }
    return res;
}
