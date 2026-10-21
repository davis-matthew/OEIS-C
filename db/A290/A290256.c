/*
 * A290256  a(n) is the number of parts equal to 1 in the integer partition
 *          having viabin number n.
 * https://oeis.org/A290256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A290255(2*n)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A290255(k) = number of 0's following directly the first 1 in the binary representation of k. */
/* This implementation uses only integer arithmetic and the helper bitlen_seq_t. */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* 2*n in seq_t range */
    seq_t m = (seq_t)n * 2u;

    /* Find the bit length of m (must be >=1 for n>=1). */
    unsigned bitlen = bitlen_seq_t(m);

    /* Count consecutive zeros starting just below the most significant bit. */
    seq_t count = 0;
    int pos = (int)bitlen - 2;          /* start at the bit below the MSB */
    while (pos >= 0) {
        if (((m >> (unsigned)pos) & 1u) != 0u)
            break;                      /* next 1 reached */
        ++count;                         /* zero found */
        --pos;
    }
    return count;
}
