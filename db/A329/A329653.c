/*
 * A329653  Describe n in binary, convert to decimal: write in base 10 the
 *          binary number concat(c0,0,c1,1) where c0, c1 is the number of '0's
 *          and '1's in n, all written in binary.
 * https://oeis.org/A329653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes OEIS A329653.
   For a given n, count the number of 0‑bits (c0) and the number of
   1‑bits (c1) in the binary representation of n (with the convention
   that the binary representation of 0 is “0”).  Then form the binary
   string:  [c0 in binary] '0' [c1 in binary] '1'  and return its
   value as an integer.  The algorithm runs in O(log n) time. */

#include <stdint.h>

/* compute the number of bits required to represent x
   (at least 1). */
static inline unsigned bitlen_uint64(uint64_t x)
{
    unsigned len = 0;
    if (x == 0) return 1;
    while (x) { ++len; x >>= 1; }
    return len;
}

/* count the number of set bits in x. */
static inline unsigned popcount_uint64(uint64_t x)
{
    unsigned cnt = 0;
    while (x) { cnt += x & 1; x >>= 1; }
    return cnt;
}

seq_t a(idx_t n)
{
    /* number of 1‑bits in n */
    unsigned ones = popcount_uint64((uint64_t)n);
    /* number of 0‑bits (with at least one 0 for n=0) */
    unsigned zeros = bitlen_uint64((uint64_t)n) - ones;

    unsigned bitlen_ones = bitlen_uint64((uint64_t)ones);
    /* shift amount needed to insert the '0' bit after zeros */
    unsigned shift = 1 + bitlen_ones;

    /* construct the binary number [c0][0][c1] */
    seq_t pre = ((seq_t)zeros << shift) + (seq_t)ones;
    /* append the final '1' bit */
    seq_t result = (pre << 1) | (seq_t)1;
    return result;
}
