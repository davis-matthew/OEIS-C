/*
 * A262438  Number of digits of hexadecimal representation of n.
 * https://oeis.org/A262438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ⌊log₁₆(n)⌋ + 1 (a(0) = 1)
 * Range:     u8 n <= 337, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"   /* provides unsigned bitlen_seq_t(seq_t) */

seq_t a(idx_t n)
{
    /* Hex length of 0 is defined to be 1 */
    if (n == 0)
        return (seq_t)1;

    /* Number of bits needed for n */
    unsigned bl = bitlen_seq_t((seq_t)n);

    /* Convert bit length to hex digit count: ceil(bl / 4) */
    return (seq_t)((bl + 3) / 4);
}
