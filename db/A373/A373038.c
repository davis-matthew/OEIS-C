/*
 * A373038  In the binary expansion of any nonnegative number n, say with
 *          Hamming weight w, reverse the order of the w rightmost bits.
 * https://oeis.org/A373038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) reverses the w rightmost bits of n where w=#ones in n
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the number of 1-bits (Hamming weight) */
    unsigned w = __builtin_popcountll((uint64_t)n);
    if (w == 0)
        return (seq_t)n;

    /* Mask for the lowest w bits.  Handle the special case w==64. */
    uint64_t mask;
    if (w == 64)
        mask = UINT64_MAX;
    else
        mask = ((uint64_t)1 << w) - 1ULL;

    /* Extract the w low bits */
    uint64_t low = (uint64_t)n & mask;

    /* Reverse those w bits */
    uint64_t rev = 0;
    for (unsigned i = 0; i < w; ++i) {
        rev = (rev << 1) | (low & 1ULL);
        low >>= 1ULL;
    }

    /* Keep the higher bits unchanged and combine */
    uint64_t high = (uint64_t)n & ~mask;
    return (seq_t)(high | rev);
}
