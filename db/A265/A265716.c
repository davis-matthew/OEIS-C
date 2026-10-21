/*
 * A265716  a(n) = n IMPL (2*n), where IMPL is the bitwise logical
 *          implication.
 * https://oeis.org/A265716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = n IMPL (2*n)
 * Range:     u8 n <= 127, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Bitwise implication is defined as (~a | b) with only the bits up to the
 * highest set bit of max(a,b).  For unsigned numbers this is equivalent to
 * ((~a) | b) & ((1 << bitlen(max(a,b))) - 1).  The function uses the
 * utility bitlen_seq_t to determine the number of bits needed.  All
 * intermediate values fit within seq_t and the operations are O(1). */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* a = n */
    seq_t y = x << 1;            /* b = 2*n */
    seq_t ma = x > y ? x : y;    /* max(a,b) */
    unsigned bl = bitlen_seq_t(ma);   /* number of bits needed for max */

    /* mask with bits 1 up to the highest set bit of max(a,b) */
    seq_t mask = bl ? ( ((seq_t)1 << bl) - 1 ) : 0;

    return ( ( (~x) | y ) & mask );
}
