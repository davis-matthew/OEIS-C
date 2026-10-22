/*
 * A320263  Write n in binary, then modify each run of 0's and each run of 1's
 *          by prepending a 0. a(n) is the decimal equivalent of the result.
 * https://oeis.org/A320263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 340, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A320263
   a(n) = decimal value of the binary obtained from the binary representation of
   n by inserting a leading 0 before each run of consecutive equal bits. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1 – not used, but safe */

    seq_t result = 0;
    int prev_bit = -1;

    /* find number of bits in n (most significant bit position + 1) */
    unsigned bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        bitlen++;
        tmp >>= 1;
    }

    /* iterate bits from most significant to least significant */
    for (int i = (int)bitlen - 1; i >= 0; --i) {
        int bit = (int)((n >> i) & 1);

        if (bit != prev_bit) {          /* start of a new run → prepend a 0 */
            result <<= 1;
        }
        result = (result << 1) | (seq_t)bit;
        prev_bit = bit;
    }

    return result;
}
