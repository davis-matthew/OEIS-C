/*
 * A358700  a(n) is the number of binary digits of n^2.
 * https://oeis.org/A358700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of binary digits of n^2
 * Range:     u8 n <= 15, u16 n <= 706, u32 n <= 9237, u64 n <= 9237,
 *            u128 n <= 9237, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sq = (seq_t)n;
    sq = sq * sq;
    return (seq_t)bitlen_seq_t(sq);
}
