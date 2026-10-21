/*
 * A261300  Concatenate successive run lengths of 0's in the binary expansion
 *          of n, each increased by 1.
 * https://oeis.org/A261300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Concatenate successive run lengths of 0's in the binary
 *            expansion of n, each increased by 1
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0   (a(0)=0) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* count number of bits in n */
    idx_t tmp = n;
    int bits = 0;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }

    seq_t result = 0;
    idx_t zero_count = 0;          /* zeros accumulated after the current '1' */

    /* scan bits from the second highest down to the lowest */
    for (int pos = bits - 2; pos >= 0; --pos) {
        bool bit = (n >> pos) & 1;
        if (bit) {
            seq_t part = (seq_t)(zero_count + 1);
            seq_t pow10 = (part < 10) ? 10 : 100;   /* part is at most 64 -> 2 decimal digits */
            result = result * pow10 + part;
            zero_count = 0;   /* reset for next run */
        } else {
            ++zero_count;
        }
    }

    /* final run of zeros after the last '1' */
    seq_t part = (seq_t)(zero_count + 1);
    seq_t pow10 = (part < 10) ? 10 : 100;
    result = result * pow10 + part;

    return result;
}
