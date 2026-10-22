/*
 * A389903  a(n) = min(n, A059893(n)), where A059893 reverses the order of all
 *          but the most significant bit in binary expansion of n.
 * https://oeis.org/A389903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = min(n, A059893(n))
 * Range:     u8 n <= 255, u16 n <= 9892, u32 n <= 9892, u64 n <= 9892,
 *            u128 n <= 9892, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A059893(n) reverses all bits of n except the most significant bit. */

#include <stdint.h>

/* Compute floor(log2(n)) for n >= 1.  */
static inline idx_t floor_log2_u64(idx_t n)
{
    /* __builtin_clzll is undefined for 0; n is guaranteed >=1. */
    return 63 - __builtin_clzll(n);
}

/* Reverse the lowest `bits` bits of `x`. */
static inline seq_t reverse_bits(seq_t x, idx_t bits)
{
    seq_t rev = 0;
    for (idx_t i = 0; i < bits; ++i)
    {
        if (x & ((seq_t)1 << i))
            rev |= ((seq_t)1 << (bits - 1 - i));
    }
    return rev;
}

/* Sequence function a(n) = min(n, A059893(n)) where n >= 1.  */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* promote index to seq_t */
    idx_t msb = floor_log2_u64(n);           /* position of most significant bit (0-based) */

    /* Bits below the most significant bit. */
    seq_t low   = nn & (((seq_t)1 << msb) - 1);
    seq_t revlow = reverse_bits(low, msb);    /* reverse them */
    seq_t rev = (((seq_t)1 << msb) | revlow); /* reattach the MSB */

    return rev > nn ? nn : rev;
}
