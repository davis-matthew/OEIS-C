/*
 * A352528  The binary expansion of a(n) is obtained by applying the
 *          elementary cellular automaton with rule (2*n) mod 256 to the
 *          binary expansion of n.
 * https://oeis.org/A352528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the result of applying elementary cellular automaton
 *            rule (2*n) mod 256 to the binary expansion of n
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* rule number (must be even) */
    uint8_t rule = (uint8_t)((n << 1) & 0xFF);

    uint64_t out64 = 0;

    for (int k = 0; k < 64; ++k) {
        uint8_t b0 = (n >> k) & 1ULL;
        uint8_t b1 = (k + 1 < 64) ? ((n >> (k + 1)) & 1ULL) : 0;
        uint8_t b2 = (k + 2 < 64) ? ((n >> (k + 2)) & 1ULL) : 0;

        uint8_t pattern = (b2 << 2) | (b1 << 1) | b0;
        if ((rule >> pattern) & 1) {
            out64 |= ((uint64_t)1 << k);
        }
    }

    return (seq_t)out64;
}
