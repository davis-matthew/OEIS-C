/*
 * A246592  Smallest number that can be obtained by swapping 2 adjacent bits
 *          in the binary expansion of n.
 * https://oeis.org/A246592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A246592: smallest number obtained by swapping the first
 *  adjacent "10" in the binary representation of n.
 *  Algorithm: scan bits from most significant to least; when the
 *  first 1 followed immediately by 0 is found, toggle those two bits
 *  (i.e. XOR with a mask having 1s in those positions).  If no such
 *  pattern exists, return n unchanged.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* no bits → nothing to swap */
        return (seq_t)0;

    /* Find position of the most significant set bit. */
    idx_t msb = 0;
    for (idx_t i = 63; i > 0; --i) {
        if ((n >> i) & 1) {
            msb = i;
            break;
        }
    }

    /* Scan from msb downwards looking for a 1 followed by a 0. */
    for (idx_t p = msb; p > 0; --p) {
        if ( ((n >> p) & 1) && !((n >> (p - 1)) & 1) ) {
            seq_t mask = ((seq_t)1 << p) | ((seq_t)1 << (p - 1));
            return (seq_t)n ^ mask;
        }
    }

    /* No "10" pattern found; return original number. */
    return (seq_t)n;
}
