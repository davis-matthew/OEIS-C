/*
 * A340632  a(n) in binary is a run of 1-bits from the most significant 1-bit
 *          of n down to the least significant 1-bit of n, inclusive.
 * https://oeis.org/A340632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ((1 << (floor_log2(n)+1)) - 1) & ~((1 << ctz(n)) - 1)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 83 terms through n = 6819 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the run of 1‑bits from the most significant to the least significant 1 in n. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* position of the highest set bit in n */
    unsigned msb = ilog2(n);
    /* position of the lowest set bit in n */
    unsigned lsb = (unsigned)__builtin_ctzll(n);

    /* total number of bits in the target type */
    const unsigned total_bits = (unsigned)(sizeof(seq_t) * 8U);

    /* high_mask has all bits set up to msb (inclusive) */
    seq_t high_mask;
    if (msb + 1 == total_bits) {
        high_mask = ~((seq_t)0);
    } else {
        high_mask = (((seq_t)1) << (msb + 1)) - 1;
    }

    /* low_mask has all bits below the lowest set bit (exclude lsb) */
    seq_t low_mask = (((seq_t)1) << lsb) - 1;

    /* set all bits from msb down to lsb to 1, retain zeros below lsb */
    return high_mask & ~low_mask;
}
