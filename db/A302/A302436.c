/*
 * A302436  a(n) is the number of ways of writing the binary expansion of n as
 *          a concatenation of nonempty substrings with Hamming weight at most
 *          1.
 * https://oeis.org/A302436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Π_{gaps between successive 1‑bits} (2^{z+1}−1) · 2^{t} *
 *            where z is the number of 0‑bits between two consecutive 1‑bits
 *            in the * binary expansion of n, and t is the number of trailing
 *            0‑bits after the * least significant 1. For n = 0 the value is
 *            defined to be 1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the index of the most significant 1‑bit (0‑based). */
static inline idx_t msb_index(idx_t x)
{
    idx_t pos = 0;
    while (x >>= 1) pos++;
    return pos;
}

/* Count trailing 0‑bits in the binary representation of x (>0). */
static inline idx_t trailing_zeros(idx_t x)
{
    idx_t tz = 0;
    while ((x & 1ULL) == 0) {
        tz++;
        x >>= 1;
    }
    return tz;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* by definition a(0) = 1 */

    /* Most significant bit index of n */
    idx_t msb = msb_index(n);

    /* Trailing zeros after the least significant 1 */
    idx_t tz = trailing_zeros(n);

    /* Start with the factor for trailing zeros: 2^tz */
    seq_t result = ((seq_t)1 << tz);

    /* Position of the last 1 processed (starting with the most significant) */
    idx_t last_pos = msb;

    /* Walk through bit positions from msb-1 down to 0 */
    for (int pos = (int)msb - 1; pos >= 0; --pos) {
        if ( (n >> pos) & 1 ) {          /* a 1‑bit is found */
            idx_t zeros_between = last_pos - pos - 1;
            seq_t factor = ((seq_t)1 << (zeros_between + 1)) - 1;
            result *= factor;
            last_pos = pos;
        }
    }

    return result;
}
