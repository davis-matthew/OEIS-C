/*
 * A374625  In the binary expansion of n, expand bits 1 -> 01 and 0 -> 10.
 * https://oeis.org/A374625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Manchester encoding of binary expansion of n (1->01,
 *            0->10)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0, the Manchester mapping produces 10 = 2. */
    if (n == 0)
        return (seq_t)2;

    seq_t result = 0;
    int bitlen = 0;
    idx_t temp = n;

    /* Determine the number of significant bits of n. */
    while (temp) {
        ++bitlen;
        temp >>= 1;
    }

    /* Process bits from most significant to least, building the Manchester
       encoded value. For each original bit: shift left by 2 and add
       pattern 01 (if the bit is 1) or 10 (if the bit is 0). */
    for (int i = bitlen - 1; i >= 0; --i) {
        seq_t bit = (seq_t)((n >> i) & 1);
        result <<= 2;
        result += (bit ? (seq_t)1 : (seq_t)2);
    }

    return result;
}
