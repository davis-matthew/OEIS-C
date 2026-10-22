/*
 * A342892  a(n) is the complement of the bit two places to the left of the
 *          least significant "1" in the binary expansion of n.
 * https://oeis.org/A342892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = complement of the bit two places to the left of the least
 *            significant 1 in the binary expansion of n
 * Range:     u8 n <= 9200, u16 n <= 9200, u32 n <= 9200, u64 n <= 9200,
 *            u128 n <= 9200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;               /* defined by the OEIS entry */

    /* Find position p of the least significant 1 (number of trailing zero bits). */
    idx_t p = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        p++;
    }

    /* Bit two places left of that 1 is at position p+2. */
    idx_t shift = p + 2;
    int bit = 0;                /* assume 0 if the position is beyond the range of n */
    if (shift < 64) {
        bit = (int)((n >> shift) & 1);
    }

    /* Complement the bit: 0 -> 1, 1 -> 0. */
    return (seq_t)(1 - bit);
}
