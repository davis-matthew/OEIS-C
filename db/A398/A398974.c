/*
 * A398974  Number of 0's in the binary expansion of n^3.
 * https://oeis.org/A398974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of 0's in binary expansion of n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2414, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* special case: 0^3 = 0 → binary "0" has one zero */
    if (n == 0)
        return (seq_t)1;

    /* compute n³ in seq_t arithmetic */
    seq_t cube = (seq_t)n;
    cube *= cube;          /* n² */
    cube *= (seq_t)n;      /* n³ */

    /* count the 1-bits in cube */
    seq_t ones = 0;
    seq_t tmp = cube;
    while (tmp != 0) {
        ones += (tmp & (seq_t)1);
        tmp >>= 1;
    }

    /* number of bits needed to represent cube */
    unsigned bitlen = bitlen_seq_t(cube);

    /* 0-bits are total bits minus 1-bits */
    seq_t zeros = (seq_t)bitlen - ones;
    return zeros;
}
