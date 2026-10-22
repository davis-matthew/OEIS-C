/*
 * A327192  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the least possible value of max(x, y).
 * https://oeis.org/A327192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = minimum over all splits of the binary representation of n
 *            * of the maximum of the two parts (values of the left and right
 *            parts)
 * Range:     u8 n <= 544, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for zero: both parts empty => max = 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the bit length of n (number of bits in its binary representation). */
    idx_t L = 0;
    idx_t t = n;
    while (t) {
        ++L;
        t >>= 1;
    }

    /* Initial best value: at worst, split at the end gives max = n. */
    seq_t best = (seq_t)n;

    /* Iterate over all split positions: j bits to the left of the split. */
    for (idx_t j = 0; j <= L; ++j) {
        idx_t shift = L - j;   /* number of bits remaining on the right side */

        /* Left part (prefix).  Handle shift==64 separately because n >> 64 is undefined. */
        seq_t x;
        if (shift == 64)
            x = (seq_t)0;
        else
            x = (seq_t)(n >> shift);

        /* Right part (suffix).  Handle shift==0 and shift==64 separately. */
        seq_t y;
        if (shift == 0)
            y = (seq_t)0;
        else if (shift == 64)
            y = (seq_t)n;
        else
            y = (seq_t)(n & (((uint64_t)1 << shift) - 1));

        /* Update best with the smaller maximum. */
        seq_t cur = (x > y) ? x : y;
        if (cur < best)
            best = cur;
    }

    return best;
}
