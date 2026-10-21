/*
 * A268716  a(n) = 2*A006068(n); main diagonal of A268714.
 * https://oeis.org/A268716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * A006068(n) where A006068 is defined by A006068(0) = 0
 *            and A006068(n) = (2 * 2^⌊log2 n⌋ - 1) - A006068(n - 2^⌊log2 n⌋)
 *            for n > 0 This recursively computes a permutation of the
 *            integers and yields a(0) = 0, a(1) = 2, a(2) = 6, etc
 * Range:     u8 n <= 127, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   _a006068
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_a006068.h"

seq_t a(idx_t n)
{
    /* a(n) = 2 * A006068(n) */
    return (seq_t)2 * _a006068(n);
}
