/*
 * A306286  a(n) is the product of the positions of the ones in the binary
 *          expansion of n (the most significant bit having position 1).
 * https://oeis.org/A306286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of the positions of the one bits in the binary
 *            expansion of n (most significant bit position 1)
 * Range:     u8 n <= 46, u16 n <= 382, u32 n <= 8190, u64 n <= 9759,
 *            u128 n <= 9759, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The empty product (n = 0) is defined to be 1 */
    if (n == 0)
        return (seq_t)1;

    /* Determine the number of bits needed to represent n */
    unsigned bitlen = 0;
    idx_t tmp = n;
    while (tmp)
    {
        tmp >>= 1;
        bitlen++;
    }

    /* Compute the product of positions of the ones, starting from the
       least significant bit (position = bitlen) and moving leftward */
    seq_t prod = (seq_t)1;
    unsigned pos = bitlen;
    while (n)
    {
        if (n & 1)
            prod *= (seq_t)pos;
        n >>= 1;
        pos--;
    }
    return prod;
}
