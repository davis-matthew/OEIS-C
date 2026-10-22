/*
 * A359359  Sum of positions of zeros in the binary expansion of n, where
 *          positions are read starting with 1 from the left (big-endian).
 * https://oeis.org/A359359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of positions of zeros in the binary expansion of n
 *            (positions start at 1 from the leftmost (MSB) bit)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: n == 0 has binary representation "0" with one zero at position 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Count number of bits in n (length of its binary representation) */
    idx_t len = 64 - __builtin_clzll(n);            /* n > 0 */

    /* Total sum of positions 1..len */
    seq_t total = (seq_t)len * (seq_t)(len + 1) / (seq_t)2;

    /* Sum of positions of 1-bits */
    seq_t sumOnes = 0;
    for (idx_t i = 0; i < len; ++i) {
        if ((n >> i) & 1ULL) {
            /* position counting from the left: len - i */
            sumOnes += (seq_t)(len - i);
        }
    }

    /* Sum of positions of zeros is total - sum of positions of ones */
    return total - sumOnes;
}
