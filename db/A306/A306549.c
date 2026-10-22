/*
 * A306549  a(n) is the product of the positions of the zeros in the binary
 *          expansion of n (the most significant bit having position 1).
 * https://oeis.org/A306549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of the positions of the zeros in the binary
 *            expansion of n, where the most significant bit has position 1.
 *            The product of an empty set is defined to be 1. For n = 0 the
 *            binary representation is "0" and the product is 1
 * Range:     u8 n <= 31, u16 n <= 255, u32 n <= 4095, u64 n <= 9567,
 *            u128 n <= 9567, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t temp = n;
    idx_t len = 0;

    /* Determine the number of bits (position of MSB) */
    while (temp) {
        ++len;
        temp >>= 1;
    }
    if (len == 0)   /* n == 0; treat as one-bit number '0' */
        len = 1;

    /* Iterate over positions 1..len; multiply by position when the bit is 0 */
    for (idx_t pos = 1; pos <= len; ++pos) {
        idx_t shift = len - pos;
        if (((n >> shift) & 1) == 0)
            prod *= pos;
    }

    return prod;
}
